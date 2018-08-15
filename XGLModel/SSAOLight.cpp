#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "SSAOCore.h"
#include "SSAOGeometry.h"
#include "Blur.h"
#include "XIOBuffer.h"
#include "SSAOLight.h"

namespace XGLModel {

	REGISTER(SSAOLight)

}

XGLModel::SSAOLight::SSAOLight()
{
	m_Mesh1 = 0;
	m_geometry = 0; 
	m_core = 0;
	m_blur = 0; 
	m_geo_buffer = 0;
	m_core_buffer= 0 ;
	m_blur_buffer = 0;
	m_pQuad = 0;
}


XGLModel::SSAOLight::~SSAOLight()
{
	if (m_Mesh1)
		delete m_Mesh1;
	m_Mesh1 = 0;

	if (m_geometry)
		delete m_geometry;
	m_geometry = 0;

	if (m_core)
		delete m_core;
	m_core = 0;

	if (m_blur)
		delete m_blur;
	m_blur = 0;

	if (m_geo_buffer)
		delete m_geo_buffer ;
	m_geo_buffer = 0;

	if (m_core_buffer)
		delete m_core_buffer;
	m_core_buffer = 0;

	if (m_blur_buffer)
		delete m_blur_buffer;
	m_blur_buffer = 0;

	if (m_pQuad)
		delete m_pQuad;
	m_pQuad = 0;

}

void XGLModel::SSAOLight::initGL()
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	m_Mesh1 = new IXMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/data/jeep.obj");

	m_pQuad = new IXMesh();
	m_pQuad->LoadMesh("E:/2018/opengl/Assimp/data/quad1.obj");

	m_geometry = new SSAOGeometry();
	m_geometry->setName("SSAOGeometry");
	m_geometry->initShader();

	m_core = new SSAOCore();
	m_core->setName("SSAOCore");
	m_core->initShader();

	m_blur = new Blur();
	m_blur->setName("Blur");
	m_blur->initShader();

	m_geo_buffer = new XIOBuffer();
	m_geo_buffer->init(windowWith,windowHeight, true, GL_RGB32F);

	m_core_buffer = new XIOBuffer();
	m_core_buffer->init(windowWith, windowHeight, false,GL_R32F);

	m_blur_buffer = new XIOBuffer();
	m_blur_buffer->init(windowWith, windowHeight, false, GL_R32F);

	glUseProgram(m_core->program);
	m_core->GenKernel();
}

void XGLModel::SSAOLight::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_mvp = glGetUniformLocation(program, "g_mvp");
	g_screensize = glGetUniformLocation(program, "g_screensize");
	g_colormap = glGetUniformLocation(program, "g_colormap");
	g_aomap = glGetUniformLocation(program, "g_aomap");

	//getErrorInformation(GetLastError());
	if (g_mv < 0|| g_mvp < 0
		|| g_screensize < 0
		|| g_colormap < 0
		|| g_aomap < 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::SSAOLight::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 5.0f, 5.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
}

void XGLModel::SSAOLight::draw()
{
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glClearColor(0, 0, 0, 0);

	m_mv = camera->getInverseMatrix();
	m_mv.preMult(Matrixf::scale(0.005f, 0.005f, 0.005f));

	geometryPass();

	corePass();
	blurPass();
	lightPass();
}

void XGLModel::SSAOLight::geometryPass()
{
	glUseProgram(m_geometry->program);
	m_geo_buffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrixf view = m_mv;
	glUniformMatrix4fv(m_geometry->g_mv, 1, GL_FALSE, view.ptr());

	view.postMult(projectMatrix);
	glUniformMatrix4fv(m_geometry->g_mvp, 1, GL_FALSE, view.ptr());
	
	m_Mesh1->Render();
}

void XGLModel::SSAOLight::corePass()
{
	glUseProgram(m_core->program);
	m_core_buffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT);

	m_geo_buffer->bindForReading(GL_TEXTURE1);
	glUniform1i(m_core->g_posmap, 1);
	glUniform1f(m_core->g_samplerrad, 0.1f);
	glUniformMatrix4fv(m_core->g_pers, 1, GL_FALSE, projectMatrix.ptr());

	m_pQuad->Render();
}

void XGLModel::SSAOLight::blurPass()
{
	glUseProgram(m_blur->program);
	m_blur_buffer->bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT);

	m_core_buffer->bindForReading(GL_TEXTURE1);
	glUniform1i(m_blur->g_aomap, 1);
	m_pQuad->Render();
}

void XGLModel::SSAOLight::lightPass()
{

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_geo_buffer->bindForBlit();
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 0, 0, 200, 200, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_core_buffer->bindForBlit();
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 200, 0, 400, 200, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_blur_buffer->bindForBlit();
	glBlitFramebuffer(0, 0, windowWith, windowHeight, 400, 0, 600, 200, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glUseProgram(program);

	Matrixf view = m_mv;
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, view.ptr());

	view.postMult(projectMatrix);
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, view.ptr());

	glUniform2f(g_screensize, windowWith, windowHeight);
	
	m_blur_buffer->bindForReading(GL_TEXTURE1);
	glUniform1i(g_aomap, 1);
	glUniform1i(g_colormap, 0);
	m_Mesh1->Render();
}
