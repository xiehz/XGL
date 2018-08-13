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
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);

	// Create the depth buffer
	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int  i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return false;
}

void XGLModel::XCubeShadowBuffer::bindForWriting(GLenum glFace)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glFace, m_shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void XGLModel::XCubeShadowBuffer::bindForTexture(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}
void XGLModel::XCubeShadowBuffer::bindForReading(GLenum texUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);

	float x = -1;
	//必须使用format对应的类型， 而不能是internal format
	/*GLAPI void GLAPIENTRY glTexImage2D(
		GLenum target, 
		GLint level, 
		GLint internalformat, //不能使用内部格式作为参数读
		GLsizei width, GLsizei height, GLint border, 
		GLenum format, //使用该格式
		GLenum type, const void *pixels);*/

	glReadPixels(256, 256, 1, 1, GL_RED, GL_FLOAT, &x);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.2, 0.2, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlitFramebuffer(0, 0, 512, 512, 0, 0, 512, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	x = -1;
	glReadPixels(256, 256, 1, 1, GL_RED, GL_FLOAT, &x);
	
}


