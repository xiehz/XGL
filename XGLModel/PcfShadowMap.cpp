#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "PcfShadowMap.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"

namespace XGLModel {
	REGISTER(PcfShadowMap)
}

XGLModel::PcfShadowMap::PcfShadowMap()
{
	m_pMesh = new IXMesh();
	m_pQuad = new IXMesh();
	m_fbo = 0;
	m_texShadow = 0;

	spotlight.Cutoff = cosf(30.f * 3.1415926f /180.0f);
	spotlight.Attenuation.Constant = 1.f;
	spotlight.Attenuation.Linear = 0.0f;
	spotlight.Attenuation.Exp = 0.0f;
	spotlight.DiffuseIntensity = 0.8;
	bkg = 0;

	m_Axes =dynamic_cast<AxesShape*>(TutorialFactory::getInstance().getClass("AxesShape"));
	m_Sphere = dynamic_cast<SphereShape*>(TutorialFactory::getInstance().getClass("SphereShape"));
	m_DepthImage = dynamic_cast<DepthImage*>(TutorialFactory::getInstance().getClass("DepthImage"));

}


XGLModel::PcfShadowMap::~PcfShadowMap()
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

void XGLModel::PcfShadowMap::draw()
{

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	static double t = 0;
	spotlight.Eposition = XGL::Vec3f( 1 *cosf(t), 0, 1 * sinf(t));
	t += 0.001;

	spotlight.Direction = -spotlight.Eposition;
	spotlight.Direction.normalize();

	glCullFace(GL_FRONT);
	renderShadow();
	glCullFace(GL_BACK);
	render();
}

void XGLModel::PcfShadowMap::initGL()
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
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_texShadow);
	glBindTexture(GL_TEXTURE_2D,m_texShadow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		windowWith,
		windowHeight,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//��ͨ�������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	//ʹ����Ӱ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

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

void XGLModel::PcfShadowMap::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_perspective = glGetUniformLocation(program, "g_pers");
	g_sampler = glGetUniformLocation(program, "g_sampler2d");
	g_lv = glGetUniformLocation(program, "g_lv");
	g_samplerShadow = glGetUniformLocation(program, "g_samplerShadow");
	g_ScreenSize = glGetUniformLocation(program, "g_MapSize");
	if (g_mv < 0
		|| g_perspective< 0
		|| g_sampler< 0
		|| g_lv < 0
		|| g_samplerShadow < 0
		|| g_ScreenSize < 0)
	{
		XGLERROR("get uniform failed");
	}
	lightShader.initUniform(program);

	//m_Axes->initUniform();
	//m_Sphere->initUniform();
}

void XGLModel::PcfShadowMap::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 10.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.f, 0.0f));
}

void XGLModel::PcfShadowMap::renderShadow()
{
	XOrbitCamera lightView;
	XGL::Vec3f zero(0,0,0);
	XGL::Vec3f up = Vec3f(0,1,0);

	lightView.setTransformation(spotlight.Eposition, zero, up);

	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());

		//ģ�ͱ任g_mv->>>ģ����С100��֮һ
		Matrixf lvMat = lightView.getInverseMatrix();
		lvMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(g_mv, 1, GL_FALSE,lvMat.ptr());

		glClear(GL_DEPTH_BUFFER_BIT);
		m_pMesh->Render();
	}

}

/*
��һ����Դλ��LightView��Ⱦ��ȣ�0~ 1��������
�ڶ��� �任ԭʼPOS�� LightView�ۿռ䣬͸�ӳ�����NDC��ӳ������0~1������ȡ�����е����ֵ���Ƚ�
�мǣ�LightView�Ŀռ�任����g_lv����ԭʼ��POS�� ģ������ı任��g_mv������û���κι���
1. �ڶ�����Ⱦ�任���ü��ռ���̣�gl_Position = g_pers * g_mv * vec4(pos, 1.0);

2. �����Դ��Ұ�±任���ü��ռ���̣�lpos = g_pers * g_lv * vec4(pos, 1.0)���ü������û��ʹ�õ�g_mv;

g_mv has nothing to do with the g_mv
*/
void XGLModel::PcfShadowMap::render()
{
	Matrixf cameraMatrix = camera->getInverseMatrix();

	XOrbitCamera lightView;
	XGL::Vec3f zero(0, 0, 0);
	XGL::Vec3f up = Vec3f(0, 1, 0);

	lightView.setTransformation(spotlight.Eposition, zero, up);
	Matrixf lvMat = lightView.getInverseMatrix();
	lvMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_DepthImage->program);
		m_DepthImage->setCamera(cameraMatrix);
		//getErrorInformation(GetLastError());
		m_DepthImage->setModel(Matrixf::scale(2.f, 2.f, 2.f )* Matrixf::translate(0.f, 4.f, -0.f));
		m_DepthImage->BindTexture(GL_TEXTURE_2D, 1, m_texShadow);
		m_DepthImage->render();

		Matrixf lightCamera =lightView.getMatrix();

		glUseProgram(m_Axes->program);
		m_Axes->setCamera(Matrixf::translate(0,0,-2));
		m_Axes->setModel(lightCamera);
		m_Axes->render();
		//--------------------------------------

		glUseProgram(m_Sphere->program);
		m_Sphere->setCamera(Matrixf::translate(0, 0, -2));
		lightCamera.preMult(Matrixf::scale(0.1, 0.1, 0.1));
		m_Sphere->setModel(lightCamera);
		m_Sphere->render();
		//--------------------------------------

		glUseProgram(program);
		glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
		glUniform2f(g_ScreenSize, windowWith, windowHeight);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texShadow);
		glUniform1i(g_samplerShadow, 1);
		glUniform1i(g_sampler, 0);
		spotlight.Eposition = spotlight.Eposition * Matrixf::translate(0, 0, -2);
		lightShader.updateUniform(spotlight, 1.0f, 16.0f);
		//ֱ��ʹ��δ��С�Ĺ�Դ��Ұ�任����Ϊquad��С�������ģ�͵İٷ�֮һ��ȡ����ʱ����
		glUniformMatrix4fv(g_lv, 1, GL_FALSE, lightView.getInverseMatrix().ptr());
		Matrixf quadMat = cameraMatrix;
		float r = 180.f * 3.1415926f / 360.0f;
		quadMat.preMult(Matrixf::rotate(XGL::Quat(sinf(r), 0, 0, cosf(r))));
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

		//��Ҫ�������Դ��Ұ��ͬ�Ĵ�����һ�κ͵ڶ��ε�����λ����ͬ��ȡ�����Ǿ��Ծ�ȷ��
		glUniformMatrix4fv(g_lv, 1, GL_FALSE, lvMat.ptr());
		Matrixf meshMat = cameraMatrix;
		meshMat.preMult(Matrixf::scale(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(g_mv, 1, GL_FALSE, meshMat.ptr());
		m_pMesh->Render();
	
}

