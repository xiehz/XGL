#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "DirLightShader.h"
#include "PointLightShader.h"
#include "DeferredShading1.h"

namespace XGLModel {

	REGISTER(DeferredShading1)

}

XGLModel::DeferredShading1::DeferredShading1()
{
	m_Mesh1 = 0;
	m_Gbuffer = 0;

	m_LightQuad = 0;
	m_LightSphere = 0;

	m_ShaderPointLight = 0;
	m_ShaderDirectionLight = 0;
}


XGLModel::DeferredShading1::~DeferredShading1()
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

	if (m_LightSphere)
		delete m_LightSphere;
	m_LightSphere = 0;

	if (m_ShaderPointLight)
		delete m_ShaderPointLight;
	m_ShaderPointLight = 0;

	if (m_ShaderDirectionLight)
		delete m_ShaderDirectionLight;
	m_ShaderDirectionLight = 0;
}

void XGLModel::DeferredShading1::initGL()
{
	m_Mesh1 = new IXMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");

	m_Gbuffer = new XGBuffer();
	m_Gbuffer->init(windowWith, windowHeight);

	initLight();
}


/*
第一个问题： 几何阶段glclearcolor 会写入fbo绑定的散射纹理，光照阶段可以影响光照计算；
第二个问题： 相机交互可以查看点光源、平行光源和模型的空间位置关系：
a. 当进入到点光源内时，光照丢失；
b. 模型不在点光源范围内， 但光源和模型的投影在屏幕上有交集时，会产生光照效果
第三个问题： 一个球对应一个点光源shader, 注意g_N 为index， shader一次只计算一个光源
*/
void XGLModel::DeferredShading1::draw()
{
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//mrt
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_Gbuffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	Matrixf mv = Matrixf::translate(XGL::Vec3f(0.0F, -10.F, 0.0F));
	mv.postMult(camera->getInverseMatrix());
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());

	Matrixf mvp = projectMatrix;
	mvp.preMult(mv);
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());
	m_Mesh1->Render();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_Gbuffer->bindForReading();
	//glDepthMask(GL_FALSE);
	//GLsizei halfwidth = windowWith / 2.0, halfheight = windowHeight / 2.0;

	//m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_POSITION);
	//glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, 0, halfwidth, halfheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	//m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_DIFFUSE);
	//glBlitFramebuffer(0, 0, windowWith, windowHeight, halfwidth, 0, windowWith, halfheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	//m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_NORMAL);
	//glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, halfheight, halfwidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	//m_Gbuffer->setReadBuffer(XGBuffer::GBUFFER_TEXTURE_TEXCOORD);
	//glBlitFramebuffer(0, 0, windowWith, windowHeight, halfwidth, halfheight, windowWith, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);


	////shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//
	m_Gbuffer->bindForSampleTexture();

	glClear(GL_COLOR_BUFFER_BIT);

	//输出到屏幕测试光照范围和空间位置
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);

	//shading point light
	glUseProgram(m_ShaderPointLight->program);

	for (int i = 0; i < m_ShaderPointLight->num(); ++i)
	{
		m_ShaderPointLight->updateLight(m_PointLight[i], i, 1.0, 32);
		float b = CalcPointLightBSphere(m_PointLight[i]);
		b = 3;
		Matrixf scale = Matrixf::scale(b, b, b);
		XGL::Vec3f world;
		if (i == 0)
		{
			world = XGL::Vec3f(2.f, 2.0f, 5.0F);
		}
		else {
			world = XGL::Vec3f(-1.0f, -4.0f, 0.0f);
		}
		XGL::Matrixf trans = Matrixf::translate(world);
		//trans.preMult(Matrixf::translate(0, 0, -15));
		trans.preMult(scale);
		trans.postMult(camera->getInverseMatrix());

		m_ShaderPointLight->updateMT(trans, projectMatrix);
		m_ShaderPointLight->updateSampler(0, 1, 2);
		m_ShaderPointLight->updateScreen(windowWith, windowHeight);
		m_LightSphere->Render();

		//确定空间位置
		//trans = Matrixf::translate(XGL::Vec3f(0.0F, -10.F ,0.0F));
		//trans.postMult(camera->getInverseMatrix());
		//m_ShaderPointLight->updateMT(trans, projectMatrix);
		//m_Mesh1->Render();
	}

	glUseProgram(m_ShaderDirectionLight->program);
	//shading direction light
	m_ShaderDirectionLight->updateLight(m_DirectionLight, 1.0, 32);
	const float degree = 90.0f;
	const float pi = 3.1415926f / 180.0f;
	const float radius = degree * pi;
	const float sin = sinf(radius), cos = cosf(radius);

	Matrixf rotate = Matrixf::rotate(XGL::Quat(sin, 0, 0, cos));
	//rotate.preMult(camera->getInverseMatrix());
	rotate.postMult(Matrixf::rotate(camera->getInverseMatrix().getRotate()));
	m_ShaderDirectionLight->updateMT(rotate, Matrixf::identity());
	m_ShaderDirectionLight->updateScreen(windowWith, windowHeight);
	m_ShaderDirectionLight->updateSampler(0, 1, 2);
	m_LightQuad->Render();
}

void XGLModel::DeferredShading1::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_mvp = glGetUniformLocation(program, "g_mvp");
}

void XGLModel::DeferredShading1::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 15.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}

void XGLModel::DeferredShading1::initLight()
{

	m_ShaderDirectionLight = new DirLightShader();
	m_ShaderDirectionLight->setName("DirLightShader");
	m_ShaderDirectionLight->initShader();

	m_DirectionLight.AmbientIntensity = 0.0f;
	m_DirectionLight.Color = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	m_DirectionLight.DiffuseIntensity = 0.1f;
	m_DirectionLight.Direction = XGL::Vec3f(0.0f, 0.0f, -1.0f);

	m_LightQuad = new IXMesh();
	m_LightQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	//--------------------------------------------------------------------------
	m_ShaderPointLight = new PointLightShader(2);
	m_ShaderPointLight->setName("PointLightShader");

	m_ShaderPointLight->initShader();

	TagAttenuation attenuation;
	attenuation.Constant = 1.0f;
	attenuation.Linear = .0f;
	attenuation.Exp = 0.f;

	Matrixf& view = camera->getInverseMatrix();

	m_PointLight[0].AmbientIntensity = 0.1f;
	m_PointLight[0].Color = XGL::Vec3f(1.0f, 0.0f, 0.0f);
	m_PointLight[0].DiffuseIntensity = 0.9f;
	m_PointLight[0].Attenuation = attenuation;
	m_PointLight[0].Eposition = view * XGL::Vec3f(2.0f, 2.0f, 5.0f);
	

	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.f;
	attenuation.Exp = .0f;
	m_PointLight[1].AmbientIntensity = 0.1f;
	m_PointLight[1].Color = XGL::Vec3f(0.0f, 0.0f, 1.0f);
	m_PointLight[1].DiffuseIntensity = 0.9f;
	m_PointLight[1].Attenuation = attenuation;
	m_PointLight[1].Eposition = view * XGL::Vec3f(-1.f, -4.f, 0.0f);

	m_LightSphere = new IXMesh();
	m_LightSphere->LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");
}

float XGLModel::DeferredShading1::CalcPointLightBSphere(const XGLModel::TagPointLight& Light)
{
	float MaxChannel = std::fmax(std::fmax(Light.Color.x(), Light.Color.y()), Light.Color.z());

	float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
		/
		(2 * Light.Attenuation.Exp);

	return ret/10;
}