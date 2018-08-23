#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "DirLightShader.h"
#include "PointLightShader.h"
#include "Background.h"
#include "DSSphere.h"

namespace XGLModel {

	REGISTER(Background)

}

XGLModel::Background::Background()
{
	m_LightQuad = 0;
	m_pTex = 0;
}


XGLModel::Background::~Background()
{

	if (m_LightQuad)
		delete m_LightQuad;
	m_LightQuad = 0;

	if (m_pTex)
		delete m_pTex;
}

void XGLModel::Background::initGL()
{
	glClearColor(.2f, .2f, 0.2f, 1.0f);
	m_LightQuad = new IXMesh();
	m_LightQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	m_pTex = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/lake1_up.JPG");
	m_pTex->Load();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

void XGLModel::Background::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glUseProgram(program);
	render();
}

void XGLModel::Background::initUniform()
{
	g_sampler = glGetUniformLocation(program, "g_sampler");
}

void XGLModel::Background::render()
{
	m_pTex->Bind(GL_TEXTURE0);
	glUniform1i(g_sampler, 0);
	m_LightQuad->Render();
}

