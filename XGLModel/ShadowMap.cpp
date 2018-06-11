#include "stdafx.h"
#include "IXMesh.h"
#include "xgl\XOrbitCamera.h"
#include "ShadowMap.h"

namespace XGLModel {

	REGISTER(ShadowMap)

}

XGLModel::ShadowMap::ShadowMap()
{
	m_pMesh = new IXMesh();
	m_pQuad = new IXMesh();
	m_fbo = 0;
	m_texShadow = 0;
}


XGLModel::ShadowMap::~ShadowMap()
{
	delete m_pMesh;
	m_pMesh = nullptr;

	delete m_pQuad;
	m_pQuad = nullptr;

	if (m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
	if (m_texShadow)
	{
		glDeleteTextures(1, &m_texShadow);
	}
}

void XGLModel::ShadowMap::initGL()
{
	glClearColor(0.341176f,0.98f,1.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	m_pMesh->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");
	m_pQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_texShadow);
	glBindTexture(GL_TEXTURE_2D, m_texShadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		windowWith,
		windowHeight,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texShadow,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		XGLERROR("attach the texute2d to framebuffer's depth Attachment failed!");
	}

}

void XGLModel::ShadowMap::draw()
{
	glUseProgram(program);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		XOrbitCamera* c = dynamic_cast<XOrbitCamera*>(camera);

		glUniformMatrix4fv(g_mv, 1, GL_FALSE, c->getInverseMatrix().ptr());
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
		m_pMesh->Render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texShadow);
		XOrbitCamera c;
		c.setTransformation(XGL::Vec3f(0.0f, 0.0f, 2.0f),
			XGL::Vec3f(0.0f, 0.0f, 0.0f),
			XGL::Vec3f(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(g_mv, 1, GL_FALSE, c.getInverseMatrix().ptr());
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
		glUniform1i(g_sampler, 0);
		m_pQuad->Render();
	}
}

void XGLModel::ShadowMap::initUniform()
{

	g_mv = glGetUniformLocation(program, "mv");
	g_perspective = glGetUniformLocation(program, "perspective");
	g_sampler = glGetUniformLocation(program, "sampler2d");


	if (g_mv < 0
		|| g_perspective< 0
		|| g_sampler< 0)
	{
		XGLERROR("get uniform failed");
	}

}

void XGLModel::ShadowMap::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 10.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	
}
