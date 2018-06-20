#include "stdafx.h"
#include "SkyCube.h"

namespace XGLModel {

	REGISTER(SkyCube)

}
XGLModel::SkyCube::SkyCube()
{
	m_pCube = nullptr;
	m_pCubeTex = nullptr;
	m_pSphere = nullptr;
	model.identity();
}


XGLModel::SkyCube::~SkyCube()
{
	if (m_pCube)
		delete m_pCube;
	m_pCube = nullptr;

	if (m_pCubeTex)
		delete m_pCubeTex;
	m_pCubeTex = nullptr;

	if (m_pSphere)
		delete m_pSphere;
	m_pSphere = nullptr;
}

void XGLModel::SkyCube::initGL()
{
	m_pSphere = dynamic_cast<SphereShape*>(TutorialFactory::getInstance().getClass("SphereShape"));
	m_pSphere->bindHandle(getHandle());
	m_pSphere->init();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	loadModel();
	Matrixf scale = Matrixf::identity();
	scale.scale(1000.f, 1000.f, 1000.f);
	setModel(scale);
}

void XGLModel::SkyCube::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render();
}

void XGLModel::SkyCube::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_cubeSampler = glGetUniformLocation(program, "g_cubeSampler");

	if (g_mv < 0
		|| g_pers< 0
		|| g_cubeSampler< 0)
	{
		XGLERROR("get uniform failed");
	}

}

void XGLModel::SkyCube::loadModel()
{
	std::string  dir = "E:/2018/opengl/Assimp/data/";
	m_pCubeTex = new XCubemapTexture(dir,
		"skycube/sp3right.jpg",
		"skycube/sp3left.jpg",
		"skycube/sp3top.jpg",
		"skycube/sp3bot.jpg",
		"skycube/sp3front.jpg",
		"skycube/sp3back.jpg");

	
	if (!m_pCubeTex->load()) {
		XGLERROR("load cube texture failed!");
	}

	m_pCube = new IXMesh();

	m_pCube->LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");

	m_pSphere->loadModel();
}

void XGLModel::SkyCube::render()
{
	Matrixf mv = camera->getInverseMatrix();
	//Matrix mv = Matrixf::identity();
	mv.setTrans(0.0f, 0.0f, 0.0f);
	mv.preMult(model);

	glUseProgram(program);
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_cubeSampler, 0);

	GLint currentCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFaceMode);
	GLint currentDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	m_pCubeTex->bindTexture(GL_TEXTURE0);
	m_pCube->Render();

	glCullFace(currentCullFaceMode);
	glDepthFunc(currentDepthFuncMode);

	glUseProgram(m_pSphere->program);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Matrixf& scale = Matrixf::scale(1.f, 1.f, 1.f);
	m_pSphere->setCamera(camera->getInverseMatrix());
	m_pSphere->setModel(scale);
	m_pSphere->render();

}

void XGLModel::SkyCube::setCamera(const XGL::Matrixf & c)
{
	camera->setByInverseMatrix(c);
}

void XGLModel::SkyCube::setModel(const XGL::Matrixf & m)
{
	model = m;
}
