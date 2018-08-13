#include "stdafx.h"
#include "CubeShape.h"

namespace XGLModel {

	REGISTER(CubeShape)

}
XGLModel::CubeShape::CubeShape()
{
	m_pCube = nullptr;
	model.identity();
}


XGLModel::CubeShape::~CubeShape()
{
	if (m_pCube)
		delete m_pCube;
	m_pCube = nullptr;
}

void XGLModel::CubeShape::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	loadModel();
	Matrixf scale = Matrixf::identity();
	scale.scale(1000.f, 1000.f, 1000.f);
	setModel(scale);
}

void XGLModel::CubeShape::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render();
}

void XGLModel::CubeShape::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_vp = glGetUniformLocation(program, "g_vp");
	g_cubeSampler = glGetUniformLocation(program, "g_cubeSampler");

	if (g_world < 0
		|| g_vp< 0
		|| g_cubeSampler< 0)
	{
		XGLERROR("get uniform failed");
	}

}

void XGLModel::CubeShape::loadModel()
{
	m_pCube = new IXMesh();

	m_pCube->LoadMesh("E:/2018/opengl/Assimp/data/box.obj");
	std::string  dir = "E:/2018/opengl/Assimp/data/";
	//m_pCubeTex = new XCubemapTexture(dir,
	//	"darkskies/darkskies_rt.tga",
	//	"darkskies/darkskies_lf.tga",
	//	"darkskies/darkskies_up.tga",
	//	"darkskies/darkskies_dn.tga",
	//	"darkskies/darkskies_ft.tga",
	//	"darkskies/darkskies_bk.tga");

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
}

void XGLModel::CubeShape::render()
{
	glUseProgram(program);
	glUniformMatrix4fv(g_world, 1, GL_FALSE, model.ptr());
	Matrixf view = camera->getInverseMatrix();
	view.postMult(projectMatrix);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, view.ptr());
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

}

void XGLModel::CubeShape::setCamera(const XGL::Matrixf & c)
{
	camera->setByInverseMatrix(c);
}

void XGLModel::CubeShape::setModel(const XGL::Matrixf & m)
{
	model = m;
}
