#include "stdafx.h"
#include "DepthImage.h"

namespace XGLModel {

	REGISTER(DepthImage)
}
XGLModel::DepthImage::DepthImage()
{
	model.identity();
}


XGLModel::DepthImage::~DepthImage()
{
}

void XGLModel::DepthImage::loadModel()
{
	m_quad.LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");
}

void XGLModel::DepthImage::BindTexture(GLenum target, GLuint texUnit, GLint texObj)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(target, texObj);
	glUniform1i(g_sampler2d, texUnit);
}

void XGLModel::DepthImage::render()
{
	Matrixf& mv = camera->getInverseMatrix();
	float r = 180.f * 3.1415926f / 360.0f;

	model.preMult(Matrixf::rotate(XGL::Quat(sinf(r), 0, 0, cosf(r))));
	mv.preMult(model);


	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());

	m_quad.Render();
}

void XGLModel::DepthImage::initGL()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	loadModel();
}

void XGLModel::DepthImage::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	render();
}

void XGLModel::DepthImage::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");

	if (g_mv < 0
		|| g_pers< 0
		|| g_sampler2d< 0)
	{
		XGLERROR("get uniform failed");
	}
}
