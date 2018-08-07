#include "stdafx.h"
#include "XGLUtil.h"
#include "XCubeShadowBuffer.h"

using namespace XGLModel;

XCubeShadowBuffer::XCubeShadowBuffer()
{
	m_fbo = 0;
	m_shadowMap = 0;
	m_depth = 0;
}


XCubeShadowBuffer::~XCubeShadowBuffer()
{
	if (m_depth)
		glDeleteTextures(1, &m_depth);
	if (m_shadowMap)
		glDeleteTextures(1, &m_shadowMap);
	if (m_fbo)
		glDeleteFramebuffers(1, &m_fbo);
}

bool XGLModel::XCubeShadowBuffer::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_fbo);

	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		XGLERROR("¼ì²éframebuffer×´Ì¬Ê§°Ü£¡");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	return false;
}

void XGLModel::XCubeShadowBuffer::bindForWriting(GLenum glFace)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glFace, m_shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void XGLModel::XCubeShadowBuffer::bindForReading(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}
