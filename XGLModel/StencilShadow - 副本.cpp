#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "XAdjacencyMesh.h"
#include "AxesShape.h"
#include "ForwardPointLightShader.h"
#include "StencilShadow.h"
#include "DSSphere.h"

namespace XGLModel {

	REGISTER(StencilShadow)

}

unsigned int XGLModel::StencilShadow::NLIGHT = 1;

XGLModel::StencilShadow::StencilShadow()
{
	m_fpLightShader = 0;
	m_pBox = 0;
	m_PointLight = 0;
	m_pAxesShader = 0;
	m_lightWorlds = 0;
	m_pDepthShader = 0;
	m_pQuad = 0;
}


XGLModel::StencilShadow::~StencilShadow()
{
	if (m_fpLightShader)
		delete m_fpLightShader;
	m_fpLightShader = 0;

	if (m_pBox)
		delete m_pBox;
	m_pBox = 0;

	if (m_PointLight)
		delete [] m_PointLight;
	m_PointLight = 0;

	if (m_pAxesShader)
		delete[] m_pAxesShader;
	m_pAxesShader = 0;

	if (m_lightWorlds)
		delete[] m_lightWorlds;
	m_lightWorlds = 0;

	if (m_pDepthShader)
		delete m_pDepthShader;
	m_pDepthShader = 0;

	if (m_pQuad)
		delete m_pQuad;
	m_pQuad = 0;

}

void XGLModel::StencilShadow::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_fpLightShader = new ForwardPointLightShader(NLIGHT);
	m_fpLightShader->setName("ForwardPointLightShader");
	m_fpLightShader->initShader();

	m_pDepthShader = new DSSphere();
	m_pDepthShader->setName("DSSphere");
	m_pDepthShader->initShader();

	m_lightWorlds = new XGL::Vec3f[NLIGHT];
	m_lightWorlds[0] = XGL::Vec3f(3.0f, 3.0f, -3.0f);
	m_lightWorlds[1] = XGL::Vec3f(-3.f, -0.f, 3.0f);

	m_PointLight = new TagPointLight[NLIGHT];

	TagAttenuation attenuation;
	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.01f;
	attenuation.Exp =.0f;

	Matrixf& view = camera->getInverseMatrix();

	m_PointLight[0].AmbientIntensity = 0.8f;
	m_PointLight[0].Color = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	m_PointLight[0].DiffuseIntensity = 0.5f;
	m_PointLight[0].Attenuation = attenuation;
	m_PointLight[0].Eposition = m_lightWorlds[0] * view;

	//attenuation.Constant = 1.0f;
	//attenuation.Linear = .5f;
	//attenuation.Exp = .0f;
	//m_PointLight[1].AmbientIntensity = 0.6f;
	//m_PointLight[1].Color = XGL::Vec3f(0.0f, 0.0f, 1.0f);
	//m_PointLight[1].DiffuseIntensity = 0.5f;
	//m_PointLight[1].Attenuation = attenuation;
	//m_PointLight[1].Eposition = m_lightWorlds[1] *view;

	m_pBox = new XAdjacencyMesh();
	m_pBox->LoadMesh("E:/2018/opengl/Assimp/data/box.obj");

	m_pQuad = new XAdjacencyMesh(false);
	m_pQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");
	m_pAxesShader = new AxesShape[NLIGHT];
	for (int i = 0; i < NLIGHT; ++i)
	{
		m_pAxesShader[i].bindHandle(getHandle());
		m_pAxesShader[i].setName("AxesShape");
		m_pAxesShader[i].init();

		m_pAxesShader[i].setCamera(view);
	}

}
Matrixf XGLModel::StencilShadow::lightToWorld(XGL::Vec3f worldposition) {
	Vec3f forward = worldposition;
	forward.normalize();

	Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

	Vec3f right = up ^ forward;
	right.normalize();

	up = forward ^ right;

	/*
	//向量基构成的矩阵：

	right.x  up.x, forward.x
	right.y  up.y, forward.y
	right.z  up.z, forward.z
	//反推c++中转置数组
	*/

	//将Axes从模型空间变换到WORLD---> 模型变换 从model -> world;
	//和实现阴影时，从光源位置渲染的modelview矩阵是完全不同的概念
	//光源位置渲染----->相机变换，从WORLD ->VIEW空间
	float light[4][4] = {
		right.x(), right. y(), right.z(), 0.0f,
		up.x(), up.y(), up.z(), 0.0f,
		forward.x(), forward.y(), forward.z(), 0.0f,
		worldposition.x(), worldposition.y(), worldposition.z(), 1.0f
	};
	return Matrixf(*light);
}

void XGLModel::StencilShadow::draw()
{

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	depthPass();

	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	shadowStencilPass();
	shadowedPass();
	glDisable(GL_STENCIL_TEST);
	ambientPass();

	Matrixf view = camera->getInverseMatrix();

	glLineWidth(1.f);
	for (int i = 0; i < NLIGHT; ++i)
	{
		glUseProgram(m_pAxesShader[i].program);
		m_pAxesShader[i].setModel(lightToWorld(m_lightWorlds[i]));
		m_pAxesShader[i].setCamera(view);
		m_pAxesShader[i].render();
	}
}

void XGLModel::StencilShadow::initUniform()
{
	g_mvp	 = glGetUniformLocation(program, "g_mvp");
	g_lightPos = glGetUniformLocation(program, "g_lightPos");
	//getErrorInformation(GetLastError());
	if (g_mvp< 0
		|| g_lightPos< 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::StencilShadow::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 10.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}

void XGLModel::StencilShadow::depthPass()
{
	Matrixf view = camera->getInverseMatrix();

	glDrawBuffer(GL_NONE);
	glUseProgram(m_pDepthShader->program);
	Matrixf mvp = projectMatrix;
	mvp.preMult(view);
	glUniformMatrix4fv(m_pDepthShader->g_mvp, 1, GL_FALSE, mvp.ptr());
	m_pBox->Render();

	float r = 90.f * 3.1415926f / 360.0f;
	Matrixf world =Matrixf::translate(0, -2, 0) ;
	world.preMult(Matrixf::rotate(XGL::Quat(sinf(r), 0, 0, cosf(r))));
	world.preMultScale(XGL::Vec3f(100.0F, 100.F, 100.F));
	mvp = projectMatrix;
	world.postMult(view);
	mvp.preMult(world);

	glUniformMatrix4fv(m_pDepthShader->g_mvp, 1, GL_FALSE, mvp.ptr());
	m_pQuad->Render();

}

void XGLModel::StencilShadow::shadowStencilPass()
{
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_CULL_FACE);

	glStencilFunc(GL_ALWAYS, 0, 0XFF);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Matrixf view = camera->getInverseMatrix();

	glUseProgram(program);
	Matrixf mvp = projectMatrix;
	mvp.preMult(view);
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());

	glUniform3f(g_lightPos, m_lightWorlds[0].x(), m_lightWorlds[0].y(), m_lightWorlds[0].z());
	m_pBox->Render();

	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
}

void XGLModel::StencilShadow::shadowedPass()
{
	Matrixf view = camera->getInverseMatrix();
	glDrawBuffer(GL_BACK);
	glStencilFunc(GL_EQUAL, 0, 0XFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


	glUseProgram(m_fpLightShader->program);
	m_fpLightShader->activeLights();
	for (int i = 0; i < NLIGHT; ++i)
	{
		m_PointLight[i].Eposition = m_lightWorlds[i] * view;
		TagPointLight light = m_PointLight[i];
		light.AmbientIntensity = 0;
		m_fpLightShader->updateLight(light, i, 1.0f, 16.0f);
	}

	m_fpLightShader->updateMT(view, projectMatrix);
	m_pBox->Render();

	float r = 90.f * 3.1415926f / 360.0f;

	Matrixf world = Matrixf::translate(0, -2, 0);
	world.preMult(Matrixf::rotate(XGL::Quat(sinf(r), 0, 0, cosf(r))));
	world.preMultScale(XGL::Vec3f(100.0F, 100.F, 100.F));
	world.postMult(view);
	m_fpLightShader->updateMT(world, projectMatrix);
	m_pQuad->Render();
}

void XGLModel::StencilShadow::ambientPass()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	Matrixf view = camera->getInverseMatrix();

	glUseProgram(m_fpLightShader->program);
	m_fpLightShader->activeLights();
	for (int i = 0; i < NLIGHT; ++i)
	{
		m_PointLight[i].Eposition = m_lightWorlds[i] * view;

		TagPointLight light = m_PointLight[i];
		light.DiffuseIntensity = 0;

		m_fpLightShader->updateLight(light, i, 0.0f, 16.0f);
	}

	m_fpLightShader->updateMT(view, projectMatrix);
	m_pBox->Render();

	float r = 90.f * 3.1415926f / 360.0f;

	Matrixf world = Matrixf::translate(0, -2, 0);
	world.preMult(Matrixf::rotate(XGL::Quat(sinf(r), 0, 0, cosf(r))));
	world.preMultScale(XGL::Vec3f(100.0F, 100.F, 100.F));
	world.postMult(view);
	m_fpLightShader->updateMT(world, projectMatrix);
	m_pQuad->Render();

	glDisable(GL_BLEND);
}
