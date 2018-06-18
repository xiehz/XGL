#include "stdafx.h"
#include "AxesShape.h"

namespace XGLModel {

	REGISTER(AxesShape)

}
XGLModel::AxesShape::AxesShape()
{
	model.identity();
}


XGLModel::AxesShape::~AxesShape()
{
}

void XGLModel::AxesShape::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	static double t = 0;
	XGL::Vec3f Eposition = XGL::Vec3f(cosf(t), 0.0f, sinf(t));
	//spotlight.Eposition = XGL::Vec3f(0.0F, 50.0f, 50.0F);

	t += 0.01;

	//XGL::Vec3f Direction = XGL::Vec3f(cosf(t), 0.0f, sinf(t));
	XGL::Vec3f Direction = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	Direction.normalize();
	//spotlight.Direction =  spotlight.Eposition;
	//spotlight.Direction.normalize();

	XGL::Vec3f right, up, forward;
	forward = Direction;
	up = Vec3f(0.0f, 1.0f, 0.0f);

	right = up ^ forward;
	right.normalize();

	up = forward ^ right;

	float lightWorld[4][4] = {
		right.x(), right.y(), right.z(),0,
		up.x(), up.y(), up.z(), 0,
		forward.x(),forward.y(),forward.z(),0,
		Eposition.x(),Eposition.y(),Eposition.z(),1.0f
	};

	model.set(*lightWorld);
	render();
}

void XGLModel::AxesShape::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	loadModel();
}

void XGLModel::AxesShape::loadModel()
{
	glPointSize(16);
	float vertices[35] = {
		0.0f, 0.0f,0.0f,
		1.0f,1.0f,1.0f,1.0f,

		1.0f , 0.0f, 0.0f,
		1.0f , 0.0f, 0.0f,1.0,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,1.0f,

		0.0f, 0.0f, 1000.f,
		0.0f, 0.0f, 1.0f,1.0f,

		1000.0F, 1000.0F, 1000.0F,
		1.0f, 0.0f, 1.0f,1.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint elements[8] = {
		0,1,
		0,2,
		0,3,
		0,4
	};
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//model.makeTranslate(0.5f, 0.5f,0.5f);
	//float deg = 22.5f * 3.1415926f / 180.0f;
	//model.preMultRotate(XGL::Quat(sin(deg),0,0,cos(deg)));
	//model.preMultScale(XGL::Vec3f(1.f, 1.f, 1.0f));



}

void XGLModel::AxesShape::setCamera(const Matrixf & m)
{
	camera->setByInverseMatrix(m);
}

void XGLModel::AxesShape::setModel(const Matrixf & m)
{
	model = m;
}
	
void XGLModel::AxesShape::render()
{

	Matrixf& mv = camera->getInverseMatrix();
	mv.preMult(model);
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT) * 3));


	glDrawArrays(GL_POINTS, 1, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void XGLModel::AxesShape::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	if (g_mv < 0
		|| g_pers< 0)
	{
		XGLERROR("get uniform failed");
	}
}
