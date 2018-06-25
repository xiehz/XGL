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
