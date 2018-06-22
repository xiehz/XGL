#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "NormalMap.h"
#include "XMesh.h"
#include "XTexture.h"
#include "AxesShape.h"
namespace XGLModel {
	REGISTER(NormalMap)
}

XGLModel::NormalMap::NormalMap()
{
	m_pBox = 0;
	m_pMaterial = 0;
	m_pNormalMap = 0;
	m_pAxes = 0;

	spotlight.Cutoff = cosf(60.f * 3.1415926f / 180.0f);

	spotlight.Attenuation.Constant = 1.f;
	spotlight.Attenuation.Linear = 0.f;
	spotlight.Attenuation.Exp = 0.00f;
}


XGLModel::NormalMap::~NormalMap()
{
	if (m_pBox)
		delete m_pBox;
	if (m_pMaterial)
		delete m_pMaterial;
	if (m_pNormalMap)
		delete m_pNormalMap;
	if (m_pAxes)
		delete m_pAxes;
}

void XGLModel::NormalMap::initGL()
{
	m_pAxes = dynamic_cast<AxesShape*>(TutorialFactory::getInstance().getClass("AxesShape"));
	m_pAxes->bindHandle(getHandle());
	m_pAxes->init();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	m_pBox = new XMesh();
	m_pBox->LoadMesh("E:/2018/opengl/Assimp/data/box.obj");

	m_pMaterial = new XTexture(GL_TEXTURE_2D,"E:/2018/opengl/Assimp/data/bricks.jpg");
	m_pMaterial->Load();
	
	m_pNormalMap = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/normal_map.jpg");
	m_pNormalMap->Load();

}

void XGLModel::NormalMap::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	Matrixf& cameraMatrix = camera->getInverseMatrix();

	static double t = 3.1415926f / 2.f;
	spotlight.Eposition = XGL::Vec3f(3* cosf(t), 0, 3 * sinf(t));
	t += 0.001;

	glUseProgram(m_pAxes->program);
	m_pAxes->setCamera(cameraMatrix);
	m_pAxes->setModel(Matrixf::translate(spotlight.Eposition));
	m_pAxes->render();

	glUseProgram(program);
	spotlight.Direction = -spotlight.Eposition;
	spotlight.Direction.normalize();


	glUniformMatrix4fv(g_mv, 1, GL_FALSE, cameraMatrix.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler2d, 0);
	glUniform1i(g_NormalMap, 1);

	spotlight.Eposition = spotlight.Eposition * cameraMatrix;
	lightShader.updateUniform(spotlight, 1.0f, 32.0f);
	m_pMaterial->Bind(GL_TEXTURE0);
	m_pNormalMap->Bind(GL_TEXTURE1);

	m_pBox->Render();
}

void XGLModel::NormalMap::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");
	g_NormalMap = glGetUniformLocation(program, "g_NormalMap");
	//getErrorInformation(GetLastError());
	if (g_mv < 0
		|| g_pers< 0
		|| g_sampler2d< 0
		|| g_NormalMap< 0)
	{
		XGLERROR("get uniform failed");
	}

	lightShader.initUniform(program);
}

void XGLModel::NormalMap::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 5.f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.f, 0.0f));
}
