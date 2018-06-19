#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "ShadowMap2.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"

namespace XGLModel {
	REGISTER(ShadowMap2)
}

XGLModel::ShadowMap2::ShadowMap2()
{
	m_pMesh = new IXMesh();
	m_pQuad = new IXMesh();
	m_fbo = 0;
	m_texShadow = 0;

	spotlight.Eposition = XGL::Vec3f(0.0f, 10.0f, 10.f);
	spotlight.Direction = XGL::Vec3f(0.0f, 0.0f, -1.0f);
	spotlight.Direction.normalize();

	spotlight.Cutoff = cosf(88.0f * 3.1415926f /180.0f);
	spotlight.Attenuation.Constant = 1.f;
	spotlight.Attenuation.Linear = 0.f;
	spotlight.Attenuation.Exp = 0.0f;
	bkg = 0;

	m_Axes =dynamic_cast<AxesShape*>(TutorialFactory::getInstance().getClass("AxesShape"));
	m_Sphere = dynamic_cast<SphereShape*>(TutorialFactory::getInstance().getClass("SphereShape"));
	m_DepthImage = dynamic_cast<DepthImage*>(TutorialFactory::getInstance().getClass("DepthImage"));

}


XGLModel::ShadowMap2::~ShadowMap2()
{
	delete m_pMesh;
	m_pMesh = nullptr;

	delete m_pQuad;
	m_pQuad = nullptr;

	if (m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
	if (m_texShadow)
	{
		glDeleteTextures(1, &m_texShadow);
	}
	if (bkg)
	{
		delete bkg;
	}

	if (m_Axes)
	{
		delete m_Axes;
		m_Axes = nullptr;
	}

	if (m_Sphere)
	{
		delete m_Sphere;
		m_Sphere = nullptr;
	}

	if (m_DepthImage)
	{
		delete m_DepthImage;
		m_DepthImage = nullptr;
	}
}

void XGLModel::ShadowMap2::draw()
{

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	static double t = 0;
	spotlight.Eposition = XGL::Vec3f( 1 *cosf(t), 0, 1 * sinf(t));
	t += 0.001;

	spotlight.Direction = spotlight.Eposition;
	spotlight.Direction.normalize();
	renderShadow();
	render();
}

void XGLModel::ShadowMap2::initGL()
{
	m_Axes->bindHandle(getHandle());
	m_Axes->init();

	m_Sphere->bindHandle(getHandle());
	m_Sphere->init();

	m_DepthImage->bindHandle(getHandle());
	m_DepthImage->init();
	glClearColor(0.341176f, 0.98f, 1.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_texShadow);
	glBindTexture(GL_TEXTURE_2D,m_texShadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		windowWith,
		windowHeight,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texShadow, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		XGLERROR("attach the texute2d to framebuffer's depth Attachment failed!");
	}

	m_pMesh->LoadMesh("E:/2018/opengl/Assimp/data/phoenix_ugv.md2");
	m_pQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");
	bkg = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/test.png");
	bkg->Load();

}

void XGLModel::ShadowMap2::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_perspective = glGetUniformLocation(program, "g_pers");
	g_sampler = glGetUniformLocation(program, "g_sampler2d");
	g_lv = glGetUniformLocation(program, "g_lv");
	g_samplerShadow = glGetUniformLocation(program, "g_samplerShadow");
	if (g_mv < 0
		|| g_perspective< 0
		|| g_sampler< 0
		|| g_lv < 0
		|| g_samplerShadow < 0)
	{
		XGLERROR("get uniform failed");
	}
	lightShader.initUniform(program);

	m_Axes->initUniform();
	m_Sphere->initUniform();
}

void XGLModel::ShadowMap2::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 10.0f, 10.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.f, 0.0f));
}

void XGLModel::ShadowMap2::renderShadow()
{
	XGL::Vec3f right, up, forward;
	forward = spotlight.Direction;
	up = Vec3f(0.0f, 1.0f, 0.0f);

	right = up ^ forward;
	right.normalize();

	up = forward ^ right;

	float lightView[4][4] = {
		right.x(), up.x(), forward.x(),0,
		right.y(), up.y(), forward.y(),0,
		right.z(), up.z(), forward.z(),0,
		-spotlight.Eposition * right , -spotlight.Eposition * up, -spotlight.Eposition * forward,1,
	};

	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
		Matrixf lvMat(*lightView);
		lvMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(g_mv, 1, GL_FALSE,lvMat.ptr());

		glClear(GL_DEPTH_BUFFER_BIT);
		m_pMesh->Render();
	}

}

/*
第一步光源位置LightView渲染深度（0~ 1）至纹理
第二步 变换原始POS至 LightView眼空间，透视除法至NDC，映射至【0~1】，提取纹理中的深度值，比较
切记：LightView的空间变换过程g_lv，是原始的POS， 模型坐标的变换是g_mv，两者没有任何关联
1. 第二次渲染变换至裁剪空间过程：gl_Position = g_pers * g_mv * vec4(pos, 1.0);

2. 计算光源视野下变换至裁剪空间过程：lpos = g_pers * g_lv * vec4(pos, 1.0)，该计算过程没有使用到g_mv;

*/
void XGLModel::ShadowMap2::render()
{
	Matrixf cameraMatrix = camera->getInverseMatrix();

	XGL::Vec3f right, up, forward;
	forward = spotlight.Direction;
	up = Vec3f(0.0f, 1.0f, 0.0f);

	right = up ^ forward;
	right.normalize();

	up = forward ^ right;

	float lightView[4][4] = {
		right.x(), up.x(), forward.x(),0,
		right.y(), up.y(), forward.y(),0,
		right.z(), up.z(), forward.z(),0,
		-spotlight.Eposition * right , -spotlight.Eposition * up, -spotlight.Eposition * forward,1,
	};
	Matrixf lvMat(*lightView);
	lvMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_DepthImage->program);
		m_DepthImage->setCamera(cameraMatrix);
		//getErrorInformation(GetLastError());
		m_DepthImage->setModel(Matrixf::scale(1.f, 1.f, 1.f )* Matrixf::translate(0.f, 2.f, -0.f));
		m_DepthImage->BindTexture(GL_TEXTURE_2D, 1, m_texShadow);
		m_DepthImage->render();
		//--------------------------------------
		float lightWorld[4][4] = {
			right.x(), right.y(), right.z(),0,
			up.x(), up.y(), up.z(), 0,
			forward.x(),forward.y(),forward.z(),0,
			spotlight.Eposition.x(),spotlight.Eposition.y(),spotlight.Eposition.z(),1.0f
		};
		Matrixf lightCamera(*lightWorld);

		glUseProgram(m_Axes->program);
		m_Axes->setCamera(cameraMatrix);
		m_Axes->setModel(lightCamera);
		m_Axes->render();
		//--------------------------------------

		glUseProgram(m_Sphere->program);
		m_Sphere->setCamera(cameraMatrix);
		lightCamera.preMult(Matrixf::scale(0.1, 0.1, 0.1));
		m_Sphere->setModel(lightCamera);
		m_Sphere->render();
		//--------------------------------------

		glUseProgram(program);
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texShadow);
		glUniform1i(g_samplerShadow, 1);
		glUniform1i(g_sampler, 0);
		spotlight.Eposition = spotlight.Eposition * cameraMatrix;
		lightShader.updateUniform(spotlight, 1.0f, 16.0f);
		glUniformMatrix4fv(g_lv, 1, GL_FALSE,*lightView);
		Matrixf quadMat = cameraMatrix;
		//Matrixf translate = Matrixf::translate(0.0f, -1.0f, -1.f);
		//quadMat.preMult(translate);
		//Matrixf scale = Matrixf::scale(10.f, 10.f, 10.f);
		//quadMat.preMult(scale);

		//float arc = 45.f* 3.1415926f / 180.0f;
		//Matrixf rotate = Matrixf::rotate(XGL::Quat(sinf(arc), 0, 0, cosf(arc)));
		//quadMat.preMult(rotate);
		glUniformMatrix4fv(g_mv, 1, GL_FALSE, quadMat.ptr());
		bkg->Bind(GL_TEXTURE0);
		m_pQuad->Render();

		glUniformMatrix4fv(g_lv, 1, GL_FALSE, lvMat.ptr());
		Matrixf meshMat = cameraMatrix;
		meshMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(g_mv, 1, GL_FALSE, meshMat.ptr());
		m_pMesh->Render();
	
}

