#include "stdafx.h"
#include "XGLUtil.h"
#include "XIOBuffer.h"

XGLModel::XIOBuffer::XIOBuffer()
{
	m_fbo = 0;
	m_depth = 0;
	m_texture = 0;
	m_internaleType = GL_NONE;
}

XGLModel::XIOBuffer::~XIOBuffer()
{
	if (m_fbo)
		glDeleteFramebuffers(1, &m_fbo);
	if (m_depth)
		glDeleteTextures(1, &m_depth);
	if (m_texture)
		glDeleteTextures(1, &m_texture);
	
	m_internaleType = GL_NONE;
}

bool XGLModel::XIOBuffer::init(unsigned int width, unsigned int height, bool withDepth, GLenum internaltype)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	m_internaleType = internaltype;

	GLenum format, type;
	switch (m_internaleType)
	{
	case GL_RGB32F:
		format = GL_RGB;
		type = GL_FLOAT;
		break;
	case GL_R32F:
		format = GL_RED;
		type = GL_FLOAT;
		break;
	case GL_NONE:
		break;
	default:
		XGLERROR("未指定合适的纹理格式！");
		break;
	}

	if (m_internaleType != GL_NONE)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, m_internaleType, width, height, 0, format, type, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
		GLenum drawbuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, drawbuffers);
	}


	if (withDepth)
	{
		glGenTextures(1, &m_depth);
		glBindTexture(GL_TEXTURE_2D, m_depth);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		XGLERROR("创建fbo失败！");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return false;
}

void XGLModel::XIOBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void XGLModel::XIOBuffer::bindForReading(GLenum texunit)
{
	glActiveTexture(texunit);
	if (m_internaleType != GL_NONE)
		glBindTexture(GL_TEXTURE_2D, m_texture);
	else
		glBindTexture(GL_TEXTURE_2D, m_depth);
}

void XGLModel::XIOBuffer::bindForBlit()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}
