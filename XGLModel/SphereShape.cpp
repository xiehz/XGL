#include "stdafx.h"
#include "xgl\Error.h"

#include "SphereShape.h"
namespace XGLModel {

	REGISTER(SphereShape)

}
XGLModel::SphereShape::SphereShape()
{
	model.identity();
}


XGLModel::SphereShape::~SphereShape()
{
}

void XGLModel::SphereShape::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	loadModel();
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

}

void XGLModel::SphereShape::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	render();

}

void XGLModel::SphereShape::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");
	//getErrorInformation(GetLastError());
	if (g_mv < 0
		|| g_pers< 0
		|| g_sampler2d< 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::SphereShape::loadModel()
{
	m_sphere.LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");
}

void XGLModel::SphereShape::setCamera(const Matrixf& m)
{
	camera->setByInverseMatrix(m);
}

void XGLModel::SphereShape::setModel(const Matrixf & m)
{
	model = m;
}

void XGLModel::SphereShape::render()
{
	Matrixf& mv = camera->getInverseMatrix();
	mv.preMult(model);

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler2d, 0);

	m_sphere.Render();
}

void XGLModel::SphereShape::setupPers()
{
	//90透视投影，刚好看向6个方向
	static float fov = 90.0f * 3.1415926f / 180.0f;
	float tanha = tanf(fov / 2);
	float aspect = (float)windowWith / (float)windowHeight;
	float nearclip = 1.0f, farclip = 10000.0f;

	double right = tanha * aspect * nearclip;
	double left = -right;
	double top = tanha * nearclip;
	double bottom = -top;
	ModelGL::project(left, right, bottom, top, nearclip, farclip);
}