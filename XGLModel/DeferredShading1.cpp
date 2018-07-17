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
float CalcPointLightBSphere(const XGLModel::TagSpotLight& Light);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_Mesh1 = new IXMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");

	m_Gbuffer = new XGBuffer();
	m_Gbuffer->init(windowWith, windowHeight);

	initLight();
}

void XGLModel::DeferredShading1::draw()
{
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	//mrt
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_Gbuffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(g_world, 1, GL_FALSE, Matrixf::identity().ptr());
	Matrixf& mv = camera->getInverseMatrix();
	Matrixf mvp = projectMatrix;
	projectMatrix.preMult(mv);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, mvp.ptr());
	m_Mesh1->Render();

	//shading
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	
	m_Gbuffer->bindForSampleTexture();
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_ShaderDirectionLight->program);
	//shading direction light
	m_ShaderDirectionLight->updateLight(m_DirectionLight,1.0, 1.0);
	m_ShaderDirectionLight->updateMT(Matrixf::identity(), Matrixf::identity());
	m_LightQuad->Render();

	//shading point light
	glUseProgram(m_ShaderPointLight->program);
	m_ShaderPointLight->activeLights();
	for (int i = 0; i < m_ShaderPointLight->num(); ++i)
	{
		m_ShaderPointLight->updateLight(m_PointLight[i], i, 1.0, 16);
		m_ShaderPointLight->updateMT(mv, projectMatrix);
		m_LightSphere->Render();
	}

}

void XGLModel::DeferredShading1::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_vp = glGetUniformLocation(program, "g_vp");
}

void XGLModel::DeferredShading1::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 5.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}

void XGLModel::DeferredShading1::initLight()
{

	m_ShaderDirectionLight = new DirLightShader();
	m_ShaderDirectionLight->setName("DirLightShader");
	m_ShaderDirectionLight->initShader();

	m_DirectionLight.AmbientIntensity = 0.1f;
	m_DirectionLight.Color = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	m_DirectionLight.DiffuseIntensity = 0.3f;
	m_DirectionLight.Direction = XGL::Vec3f(0.0f, -1.0f, 0.0f);

	m_LightQuad = new IXMesh();
	m_LightQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	//--------------------------------------------------------------------------
	m_ShaderPointLight = new PointLightShader(2);
	m_ShaderPointLight->setName("PointLightShader");

	m_ShaderPointLight->initShader();

	TagAttenuation attenuation;
	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.1f;
	attenuation.Exp = 0.01f;

	m_PointLight[0].AmbientIntensity = 0.0f;
	m_PointLight[0].Color = XGL::Vec3f(1.0f, 0.0f, 0.0f);
	m_PointLight[0].DiffuseIntensity = 0.6f;
	m_PointLight[0].Attenuation = attenuation;
	m_PointLight[0].Eposition = XGL::Vec3f(2.0f, 2.0f, 0.0f);
	
	m_PointLight[1].AmbientIntensity = 0.0f;
	m_PointLight[1].Color = XGL::Vec3f(0.0f, 0.0f, 1.0f);
	m_PointLight[1].DiffuseIntensity = 0.6f;
	m_PointLight[1].Attenuation = attenuation;
	m_PointLight[1].Eposition = XGL::Vec3f(-2.0f, 0.0f, 0.0f);

	m_LightSphere = new IXMesh();
	m_LightSphere->LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");
}

float CalcPointLightBSphere(const XGLModel::TagSpotLight& Light)
{
	float MaxChannel = std::fmax(std::fmax(Light.Color.x(), Light.Color.y()), Light.Color.z());

	float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
		/
		(2 * Light.Attenuation.Exp);

	return ret;
}