#include "stdafx.h"
#include "Picking.h"
#include "SphereShape.h"
#include "PickingDemo.h"

namespace XGLModel {

	REGISTER(PickingDemo)

}

XGLModel::PickingDemo::PickingDemo()
{
	m_picking = 0;
	m_hightlight = 0;
}


XGLModel::PickingDemo::~PickingDemo()
{
	if (m_picking)
		delete m_picking;
	m_picking = 0;
	if (m_hightlight)
		delete m_hightlight;
	m_hightlight = 0;
}

void XGLModel::PickingDemo::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_model.LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");
	m_picking = new Picking();
	m_picking->setName("Picking");
	m_picking->bindHandle(getHandle());
	m_picking->init();

	m_model.AcceptPicking(m_picking);

	m_hightlight = new SphereShape();
	m_hightlight->setName("SphereShape");
	m_hightlight->bindHandle(getHandle());
	m_hightlight->initShader();
	m_hightlight->initUniform();



}

void XGLModel::PickingDemo::draw()
{
	drawPicking();

	//±ØÐëÔÚunbindÇ°read
	m_picking->readPixel(400, 400);

	drawScene(m_picking->m_pixel);
}
void XGLModel::PickingDemo::drawPicking() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_picking->m_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_picking->program);
	const Matrixf& mv = camera->getInverseMatrix();

	glUniformMatrix4fv(m_picking->g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(m_picking->g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1ui(m_picking->g_objectindex, 0);
	m_model.Render();

	Matrixf world = camera->getInverseMatrix();
	world.preMult(Matrixf::translate(5.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(m_picking->g_mv, 1, GL_FALSE, world.ptr());
	glUniformMatrix4fv(m_picking->g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1ui(m_picking->g_objectindex, 1);
	m_model.Render();

}

void XGLModel::PickingDemo::drawScene(const XGLModel::PixelInfo& pixel) {
	//
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);

	const Matrixf& mv = camera->getInverseMatrix();
	Matrixf world = camera->getInverseMatrix();
	world.preMult(Matrixf::translate(5.0f, 0.0f, 0.0f));

	glUseProgram(program);
	glUniform1i(g_sampler2d, 0);
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	m_model.Render();

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, world.ptr());
	m_model.Render();

	//glBlitFramebuffer(300, 300, 500, 500, 0, 0, 200, 200, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	if (pixel.PrimitiveID >= 1)
	{
		glPolygonMode(GL_FRONT, GL_FILL);

		glUseProgram(m_hightlight->program);
		glUniformMatrix4fv(m_hightlight->g_pers, 1, GL_FALSE, projectMatrix.ptr());
		glUniform1i(m_hightlight->g_sampler2d, 0);

		if (pixel.ObjectID == 0)
		{
			glUniformMatrix4fv(m_hightlight->g_mv, 1, GL_FALSE, mv.ptr());
		}
		else if (pixel.ObjectID == 1)
		{
			glUniformMatrix4fv(m_hightlight->g_mv, 1, GL_FALSE, world.ptr());
		}
		else {

		}
		m_model.Render(pixel.DrawID, pixel.PrimitiveID - 1);
	}
	glBlitFramebuffer(300, 300, 500, 500, 0, 0, 200, 200, GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

void XGLModel::PickingDemo::initUniform()
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
