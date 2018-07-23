#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "DeferredShading0.h"

namespace XGLModel {

	REGISTER(DeferredShading0)

}

XGLModel::DeferredShading0::DeferredShading0()
{
	m_Mesh1 = 0;
	m_Gbuffer = 0;
}


XGLModel::DeferredShading0::~DeferredShading0()
{
	if (m_Mesh1)
		delete m_Mesh1;
	m_Mesh1 = 0;

	if (m_Gbuffer)
		delete m_Gbuffer;
	m_Gbuffer = 0;
}

void XGLModel::DeferredShading0::initGL()
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	m_Mesh1 = new IXMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");
	//m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/data/phoenix_ugv.md2");

	m_Gbuffer = new XGBuffer();
	m_Gbuffer->init(windowWith, windowHeight);
}

void XGLModel::DeferredShading0::draw()
{
	//mrt
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	m_Gbuffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrixf mvp = projectMatrix;
	mvp.preMult(camera->getInverseMatrix());
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, camera->getInverseMatrix().ptr());
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());
	m_Mesh1->Render();

	//
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	m_Gbuffer->bindForReading();
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//不能禁止深度写入
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	GLsizei halfwidth = windowWith / 2.0, halfheight = windowHeight /2.0;

	m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_POSITION);
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, 0, halfwidth, halfheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_DIFFUSE);
	glBlitFramebuffer(0, 0, windowWith, windowHeight, halfwidth, 0, windowWith, halfheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_NORMAL);
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, halfheight, halfwidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_TEXCOORD);
	glBlitFramebuffer(0, 0, windowWith, windowHeight, halfwidth, halfheight, windowWith, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void XGLModel::DeferredShading0::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_mvp = glGetUniformLocation(program, "g_mvp");
	//getErrorInformation(GetLastError());
	if (g_mv < 0|| g_mvp < 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::DeferredShading0::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 26.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}
