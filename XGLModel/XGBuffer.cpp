#include "stdafx.h"
#include "xgl/Error.h"
#include "XGLUtil.h"
#include "XGLModel.h"
#include "XGBuffer.h"


XGLModel::XGBuffer::XGBuffer()
{
	m_fbo = 0;
	m_depthTex = 0;
	memset(m_textures, 0, sizeof(m_textures));
}


XGLModel::XGBuffer::~XGBuffer()
{
	if (m_fbo)
		glDeleteFramebuffers(1, &m_fbo);
	m_fbo = 0;
	
	if (m_textures[0])
		glDeleteTextures(4, m_textures);

	if (m_depthTex)
		glDeleteTextures(1, &m_depthTex);
}

bool XGLModel::XGBuffer::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glGenTextures(4, m_textures);
	for (unsigned int i = 0; i < 4; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	glGenTextures(1, &m_depthTex);
	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, m_depthTex, 0);

	GLenum drawbuffer[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		XGLERROR((char*)glewGetErrorString(status));
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void XGLModel::XGBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void XGLModel::XGBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void XGLModel::XGBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE gbuffer_texture)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + gbuffer_texture);
}
