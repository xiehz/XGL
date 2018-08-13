#include "stdafx.h"
#include <assert.h>
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "MultiChannelShadowMap.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"
#include "XCubeShadowBuffer.h"
#include "CubeShadowMap.h"
#include "XCubemapTexture.h"
#include "CubeShape.h"

namespace XGLModel {
	REGISTER(MultiChannelShadowMap)
}

int XGLModel::MultiChannelShadowMap::N = 1;

XGLModel::MultiChannelShadowMap::MultiChannelShadowMap()
{
	m_pMesh = new IXMesh();
	m_pQuad = new IXMesh();
	m_pCubeBuffer = 0;
	m_cubeShadowShader = 0;

	m_Sphere = dynamic_cast<SphereShape*>(TutorialFactory::getInstance().getClass("SphereShape"));
	m_pointLocation = 0;
	m_PointLight = 0;
	m_pTexture = 0;
	m_CubeShape = 0;
}


XGLModel::MultiChannelShadowMap::~MultiChannelShadowMap()
{
	delete m_pMesh;
	m_pMesh = nullptr;

	delete m_pQuad;
	m_pQuad = nullptr;

	if (m_pCubeBuffer)
		delete m_pCubeBuffer;
	m_pCubeBuffer = 0;

	if (m_Sphere)
	{
		delete m_Sphere;
		m_Sphere = nullptr;
	}

	if (m_cubeShadowShader)
		delete m_cubeShadowShader;
	m_cubeShadowShader = 0;

	if(m_pointLocation)
		delete[] m_pointLocation;
	if (m_PointLight)
		delete[] m_PointLight;

	if(m_pTexture)
		delete m_pTexture;

	if (m_CubeShape)
		delete m_CubeShape;
}

void XGLModel::MultiChannelShadowMap::draw()
{
	glCullFace(GL_FRONT);
	renderShadow();
	glCullFace(GL_BACK);
	render();
}

void XGLModel::MultiChannelShadowMap::initGL()
{
	std::ostringstream stream;
	stream << "\nVendor: " << glGetString(GL_VENDOR) << ".";
	stream << "\nRenderer: " << glGetString(GL_RENDERER) << ".";
	stream << "\nVersion: " << glGetString(GL_VERSION) << ".";
	stream << "\nGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ".";

	XGLERROR(stream.str().c_str());
	stream.clear();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, 512, 512);

	m_Sphere->bindHandle(getHandle());
	m_Sphere->init();

	m_cubeShadowShader = new CubeShadowMap();
	m_cubeShadowShader->bindHandle(getHandle());
	m_cubeShadowShader->setName("CubeShadowMap");
	m_cubeShadowShader->init();

	m_CubeShape = new CubeShape();
	m_CubeShape->setName("CubeShape");
	m_CubeShape->initShader();
	m_CubeShape->loadModel();

	m_pMesh->LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");
	m_pQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad1.obj");

	m_pCubeBuffer = new XCubeShadowBuffer();
	m_pCubeBuffer->init(512 , 512);

	m_PointLight = new TagPointLight[N];

	TagAttenuation attenuation;
	attenuation.Constant = 1.0f;
	attenuation.Linear = 0.0f;
	attenuation.Exp = .0f;


	m_PointLight[0].AmbientIntensity = 0.1f;
	m_PointLight[0].Color = XGL::Vec3f(1.0f, 1.0f, 1.0f);
	m_PointLight[0].DiffuseIntensity = 1.f;
	m_PointLight[0].Attenuation = attenuation;
	m_PointLight[0].Eposition = XGL::Vec3f(0.0, 0.0, 0.0f);

	m_pointLocation = new TagPointLightLocation[N];

	m_pTexture = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/test.png");
	m_pTexture->Load();

	std::string  dir = "E:/2018/opengl/Assimp/data/";
	m_pCubeTex = new XCubemapTexture(dir,
		"skycube/sp3right.jpg",
		"skycube/sp3left.jpg",
		"skycube/sp3top.jpg",
		"skycube/sp3bot.jpg",
		"skycube/sp3front.jpg",
		"skycube/sp3back.jpg");
	m_pCubeTex->load();

}


void XGLModel::MultiChannelShadowMap::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 10.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.f, 0.0f));
}

void XGLModel::MultiChannelShadowMap::setupPers()
{
	//90透视投影，刚好看向6个方向
	static float fov = 90.0f * 3.1415926f / 180.0f;
	float tanha = tanf(fov / 2);
	float aspect = (float)512 / (float)512;
	float nearclip = 1.0f, farclip = 10000.0f;

	double right = tanha * aspect * nearclip;
	double left = -right;
	double top = tanha * nearclip;
	double bottom = -top;
	ModelGL::project(left, right, bottom, top, nearclip, farclip);
}

void XGLModel::MultiChannelShadowMap::renderShadow()
{
	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

	glUseProgram(m_cubeShadowShader->program);
	for (int lightnum = 0; lightnum < N; ++lightnum)
	{
		for (int i = 0; i < 6; ++i)
		{
			m_pCubeBuffer->bindForWriting(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			m_cubeShadowShader->setupPers(m_PointLight[lightnum].Eposition,i);

			Matrixf world = Matrixf::translate(0, 0, -2);
			m_cubeShadowShader->setWorld( world);
			m_pMesh->Render();

			world = Matrixf::translate(-2.5f, -1.0f, -3.0f);
			m_cubeShadowShader->setWorld( world);
			m_pMesh->Render();
		}
	}


}

/*
第一步光源位置LightView渲染深度（0~ 1）至纹理
第二步 变换原始POS至 LightView眼空间，透视除法至NDC，映射至【0~1】，提取纹理中的深度值，比较
切记：LightView的空间变换过程g_lv，是原始的POS， 模型坐标的变换是g_mv，两者没有任何关联
1. 第二次渲染变换至裁剪空间过程：gl_Position = g_pers * g_mv * vec4(pos, 1.0);

2. 计算光源视野下变换至裁剪空间过程：lpos = g_pers * g_lv * vec4(pos, 1.0)，该计算过程没有使用到g_mv;

g_mv has nothing to do with the g_mv
*/
void XGLModel::MultiChannelShadowMap::render()
{
	Matrixf cameraMatrix = camera->getInverseMatrix();
	setupPers();

	m_pCubeBuffer->bindForTexture(GL_TEXTURE1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.2, 0.2, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//------------测试渲染两个球在点光源下的深度纹理
	//glUseProgram(m_CubeShape->program);
	//glUniformMatrix4fv(m_CubeShape->g_world, 1, GL_FALSE, Matrixf::scale(5.12f, 5.12f, 5.12f).ptr());
	//Matrixf view = cameraMatrix;
	//view.postMult(projectMatrix);
	//glUniformMatrix4fv(m_CubeShape->g_vp, 1, GL_FALSE, view.ptr());
	////m_pCubeTex->bindTexture(GL_TEXTURE1);
	//glUniform1i(m_CubeShape->g_cubeSampler, 1);

	//m_CubeShape->m_pCube->Render();
	//return;

	glUseProgram(program);
	glUniform1ui(g_N, N);
	m_pTexture->Bind(GL_TEXTURE0);
	glUniform1i(g_sampler, 0);

	glUniform1i(g_shadowmap, 1);
	Matrixf vp = projectMatrix;
	vp.preMult(cameraMatrix);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, vp.ptr());

	for (int lightnum = 0; lightnum < N; ++lightnum)
	{
		TagPointLight& poingLight = m_PointLight[lightnum];
		glUniform3f(g_eye_world, 0.0f, 0.0f, 10.0f);

		updateLight(poingLight, lightnum, 1.0, 16.0);

		Matrixf world = Matrixf::translate(0, 0, -2);
		glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
		m_pMesh->Render();

		world = Matrixf::translate(-2.5f, -1.0f, -3.0f);
		glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
		m_pMesh->Render();

		world = Matrixf::scale(10.0f, 10.0f ,10.0f);
		world.postMult(Matrixf::translate(0, 0, -10));
		glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
		m_pQuad->Render();

		float rad = 45.0f * 3.1415926f / 180.0f;
		float rsin = sinf(rad) ,rcos = cosf(rad);

		world = Matrixf::scale(10.0f, 10.0f, 10.0f);
		world.postMultRotate(XGL::Quat(-rsin, 0, 0, rcos));
		world.postMult(Matrixf::translate(0, -10, 0));
		glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
		m_pQuad->Render();

		world = Matrixf::scale(10.0f, 10.0f, 10.0f);
		world.postMultRotate(XGL::Quat(0, rsin, 0, rcos));
		world.postMult(Matrixf::translate(-10, 0, 0));
		glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
		m_pQuad->Render();
	}

	//--------------------------------------
	glUseProgram(m_Sphere->program);
	for (int lightnum = 0; lightnum < N; ++lightnum)
	{
		Matrixf lightCamera = Matrixf::translate(m_PointLight[lightnum].Eposition);
		m_Sphere->setupPers();
		m_Sphere->setCamera(cameraMatrix);
		//	lightCamera.preMult(Matrixf::scale(0.1, 0.1, 0.1));
		m_Sphere->setModel(lightCamera);
		m_Sphere->render();
	}

	//--------------------------------------
}

void XGLModel::MultiChannelShadowMap::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_vp = glGetUniformLocation(program, "g_vp");
	g_sampler = glGetUniformLocation(program, "g_sampler2d");
	g_eye_world = glGetUniformLocation(program, "g_eye_wordpos");
	g_shadowmap = glGetUniformLocation(program, "g_shadowmap");
	if (g_world < 0
		|| g_vp< 0
		|| g_sampler< 0
		|| g_eye_world < 0
		|| g_shadowmap < 0)
	{
		XGLERROR("get uniform failed");
	}

	for (int i = 0; i < N; ++i)
	{
		std::ostringstream stream;
		stream << "g_pointlight[" << i << "].";
		std::string prefix = stream.str();
		stream.clear();

		std::string name = prefix + "Color";
		m_pointLocation[i].Color = glGetUniformLocation(program, name.c_str());

		name = prefix + "AmbientIntensity";
		m_pointLocation[i].AmbientIntensity = glGetUniformLocation(program, name.c_str());

		name = prefix + "DiffuseIntensity";
		m_pointLocation[i].DiffuseIntensity = glGetUniformLocation(program, name.c_str());

		name = prefix + "Eposition";
		m_pointLocation[i].Eposition = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Constant";
		m_pointLocation[i].Attenuation.Constant = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Linear";
		m_pointLocation[i].Attenuation.Linear = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Exp";
		m_pointLocation[i].Attenuation.Exp = glGetUniformLocation(program, name.c_str());

	}
	g_shineness = glGetUniformLocation(program, "g_shineness");
	g_materialIntensity = glGetUniformLocation(program, "g_materialIntensity");
	g_N = glGetUniformLocation(program, "g_N");
}

void XGLModel::MultiChannelShadowMap::activeLights()
{
	glUniform1ui(g_N, N);
}

void XGLModel::MultiChannelShadowMap::updateLight(const TagPointLight& pl, const unsigned int index, float materialIntensity, float shineness)
{
	assert(index < N);

	glUniform1f(m_pointLocation[index].AmbientIntensity, pl.AmbientIntensity);
	glUniform1f(m_pointLocation[index].DiffuseIntensity, pl.DiffuseIntensity);
	glUniform3f(m_pointLocation[index].Color, pl.Color.x(), pl.Color.y(), pl.Color.z());
	glUniform3f(m_pointLocation[index].Eposition, pl.Eposition.x(), pl.Eposition.x(), pl.Eposition.x());
	glUniform1f(m_pointLocation[index].Attenuation.Constant, pl.Attenuation.Constant);
	glUniform1f(m_pointLocation[index].Attenuation.Linear, pl.Attenuation.Linear);
	glUniform1f(m_pointLocation[index].Attenuation.Exp, pl.Attenuation.Exp);
	glUniform1f(g_shineness, shineness);
	glUniform1f(g_materialIntensity, materialIntensity);

}

