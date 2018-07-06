#include "stdafx.h"
#include "xgl\Error.h"
#include "Picking.h"


XGLModel::Picking::Picking()
{
	m_fbo = 0;
	m_colorbuffer = 0;
	m_depthbuffer = 0;
}


XGLModel::Picking::~Picking()
{
	if (m_fbo)
		glDeleteFramebuffers(1, &m_fbo);
	if (m_colorbuffer)
		glDeleteRenderbuffers(1, &m_colorbuffer);
	if (m_depthbuffer)
		glDeleteRenderbuffers(1, &m_depthbuffer);
}

void XGLModel::Picking::initGL()
{
	glDisable(GL_BLEND);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenRenderbuffers(1, &m_colorbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_colorbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32UI, windowWith, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorbuffer);

	glGenRenderbuffers(1, &m_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWith, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		XGLERROR("check framebuffer status failed");
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void XGLModel::Picking::draw()
{

}

void XGLModel::Picking::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_objectindex = glGetUniformLocation(program, "g_objectindex");
	g_drawcallindex = glGetUniformLocation(program, "g_drawcallindex");
	if (g_mv < 0||
		g_pers < 0 ||
		g_objectindex < 0|| 
		g_drawcallindex <0
	)
	{
		XGLERROR("get picking uniform failed");
	}
}

void XGLModel::Picking::setCamera(const Matrixf & m)
{
	camera->setByInverseMatrix(m);
}

const XGLModel::PixelInfo& XGLModel::Picking::readPixel(unsigned int x, unsigned int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &m_pixel);

	glReadBuffer(GL_DEPTH_ATTACHMENT);
	float depth;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	
	//Matrixf mvp = projectMatrix;
	//mvp.preMult(camera->getInverseMatrix());
	//mvp = Matrixf::inverse(mvp);

	//XGL::Vec3f center = Vec3f(0, 0, depth);
	////center = mvp.preMult(center);
	//center = center * mvp;

	glReadBuffer(GL_NONE);
	return m_pixel;
}
