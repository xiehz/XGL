#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "DirLightShader.h"
#include "PointLightShader.h"
#include "DeferredShading2.h"
#include "DSSphere.h"

namespace XGLModel {

	REGISTER(DeferredShading2)

}

XGLModel::DeferredShading2::DeferredShading2()
{
	m_Mesh1 = 0;
	m_Gbuffer = 0;

	m_LightQuad = 0;

	m_ShaderPointLight = 0;
	m_ShaderDirectionLight = 0;

	m_dsSphere[0] = 0;
	m_dsSphere[1] = 0;
}


XGLModel::DeferredShading2::~DeferredShading2()
{
	if (m_Mesh1)
		delete m_Mesh1;
	m_Mesh1 = 0;

	if (m_Gbuffer)
		delete m_Gbuffer;
	m_Gbuffer = 0;

	if (m_LightQuad)
		delete m_LightQuad;
	m_LightQuad = 0;

	if (m_ShaderPointLight)
		delete m_ShaderPointLight;
	m_ShaderPointLight = 0;

	if (m_ShaderDirectionLight)
		delete m_ShaderDirectionLight;
	m_ShaderDirectionLight = 0;
	
	for (int i = 0; i < 2; ++i)
	{
		delete m_dsSphere[i];
		m_dsSphere[i] = 0;
	}
}

void XGLModel::DeferredShading2::initGL()
{
	m_Mesh1 = new IXMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");

	m_Gbuffer = new XGBuffer();
	m_Gbuffer->initPass(windowWith, windowHeight);

	initLight();
}


/*
第一个问题： 几何阶段glclearcolor 会写入fbo绑定的散射纹理，光照阶段可以影响光照计算；
第二个问题： 相机交互可以查看点光源、平行光源和模型的空间位置关系：
a. 当进入到点光源内时，光照丢失；
b. 模型不在点光源范围内， 但光源和模型的投影在屏幕上有交集时，会产生光照效果
第三个问题： 一个球对应一个点光源shader, 注意g_N 为index， shader一次只计算一个光源
*/
void XGLModel::DeferredShading2::draw()
{
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	m_Gbuffer->startFrame();
	dsGeometryPass();

	glEnable(GL_STENCIL_TEST);
	for (int i = 0; i < 2; ++i)
	{
		dsStencilPass(i);
		dsPointLightPass(i);
	}
	glDisable(GL_STENCIL_TEST);

	dsDirectionLightPass();
	dsFinalPass();
}

void XGLModel::DeferredShading2::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_mvp = glGetUniformLocation(program, "g_mvp");
}

void XGLModel::DeferredShading2::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 15.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}

void XGLModel::DeferredShading2::dsGeometryPass()
{
	m_Gbuffer->bindGeoPass();

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	//mrt
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	Matrixf mv = Matrixf::translate(XGL::Vec3f(0.0F, -10.F, 0.0F));

	mv.postMult(camera->getInverseMatrix());
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());

	Matrixf mvp = projectMatrix;
	mvp.preMult(mv);
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());
	m_Mesh1->Render();

	glDepthMask(GL_FALSE);

}

void XGLModel::DeferredShading2::dsStencilPass(unsigned int index)
{
	m_Gbuffer->bindStencilPass();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS,0,0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	glUseProgram(m_dsSphere[index]->program);
	m_dsSphere[index]->setCamera(camera->getInverseMatrix());
	m_dsSphere[index]->draw();
}

void XGLModel::DeferredShading2::dsDirectionLightPass()
{
	m_Gbuffer->bindLightPass();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glUseProgram(m_ShaderDirectionLight->program);
	//shading direction light
	m_ShaderDirectionLight->updateLight(m_DirectionLight, 1.0, 32);
	const float degree = 90.0f;
	const float pi = 3.1415926f / 180.0f;
	const float radius = degree * pi;
	const float sin = sinf(radius), cos = cosf(radius);

	Matrixf rotate = Matrixf::rotate(XGL::Quat(sin, 0, 0, cos));
	rotate.postMult(Matrixf::rotate(camera->getInverseMatrix().getRotate()));
	m_ShaderDirectionLight->updateMT(rotate, Matrixf::identity());
	m_ShaderDirectionLight->updateScreen(windowWith, windowHeight);
	m_ShaderDirectionLight->updateSampler(0, 1, 2);
	m_LightQuad->Render();

	glDisable(GL_BLEND);
}

void XGLModel::DeferredShading2::dsPointLightPass(unsigned int index)
{
	////shading
	m_Gbuffer->bindLightPass();
	glStencilFunc(GL_NOTEQUAL, 0, 0xff);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	//只背面绘制，解决视点在球体内，无光照
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glUseProgram(m_ShaderPointLight->program);

	m_ShaderPointLight->updateLight(m_PointLight[index], index, 1.0, 32);
	Matrixf mv = camera->getInverseMatrix();
	mv.preMult(m_dsSphere[index]->m_WorldMT);
	m_ShaderPointLight->updateMT(mv, projectMatrix);
	m_ShaderPointLight->updateSampler(0, 1, 2);
	m_ShaderPointLight->updateScreen(windowWith, windowHeight);
	m_dsSphere[index]->m_LightSphere->Render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void XGLModel::DeferredShading2::dsFinalPass()
{
	m_Gbuffer->bindFinalPass();
	glClearColor(0.0f, 0.0f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, 0, windowWith, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void XGLModel::DeferredShading2::initLight()
{
	m_ShaderDirectionLight = new DirLightShader();
	m_ShaderDirectionLight->setName("DirLightShader");
	m_ShaderDirectionLight->initShader();

	m_DirectionLight.AmbientIntensity = 0.0f;
	m_DirectionLight.Color = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	m_DirectionLight.DiffuseIntensity = 0.2f;
	m_DirectionLight.Direction = XGL::Vec3f(0.0f, 0.0f, -1.0f);

	m_LightQuad = new IXMesh();
	m_LightQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	//--------------------------------------------------------------------------
	m_ShaderPointLight = new PointLightShader(2);
	m_ShaderPointLight->setName("PointLightShader");

	m_ShaderPointLight->initShader();

	TagAttenuation attenuation;
	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.0f;
	attenuation.Exp = 0;

	Matrixf& view = camera->getInverseMatrix();

	m_PointLight[0].AmbientIntensity = 0.1f;
	m_PointLight[0].Color = XGL::Vec3f(1.0f, .0f, .0f);
	m_PointLight[0].DiffuseIntensity = 1.f;
	m_PointLight[0].Attenuation = attenuation;
	m_PointLight[0].Eposition =  XGL::Vec3f(2.0f, -0.0f, 5.0f) * view;

	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.f;
	attenuation.Exp = .0f;
	m_PointLight[1].AmbientIntensity = 0.1f;
	m_PointLight[1].Color = XGL::Vec3f(0.0f, 0.0f, 1.0f);
	m_PointLight[1].DiffuseIntensity = 1.f;
	m_PointLight[1].Attenuation = attenuation;
	m_PointLight[1].Eposition = XGL::Vec3f(-3.f, -0.f, 0.0f) *view;

	for (int i = 0; i < 2; ++i)
	{
		m_dsSphere[i] = new DSSphere();
		m_dsSphere[i]->setName("DSSphere");
		m_dsSphere[i]->bindHandle(getHandle());
		m_dsSphere[i]->init();
		m_dsSphere[i]->setCamera(camera->getInverseMatrix());
		m_dsSphere[i]->setModelByPointLight(m_PointLight[i]);
	}
}
