#include "stdafx.h"
#include "xgl\Math"
#include "XTexture.h"
#include "XPatchMesh.h"
#include "PNTessellation.h"

namespace XGLModel {
	REGISTER(PNTessellation)
}
const float rad = -45.f * 3.1415926f / 360.f;

XGLModel::PNTessellation::PNTessellation()
{
	m_patchMesh = 0;
	m_heightMap = 0;
}


XGLModel::PNTessellation::~PNTessellation()
{
	if (m_patchMesh)
		delete m_patchMesh;
	m_patchMesh = 0;
	if (m_heightMap)
		delete m_heightMap;
	m_heightMap = 0;
}

void XGLModel::PNTessellation::initGL()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_patchMesh = new XPatchMesh();
	m_patchMesh->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	m_heightMap = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/heightmap.jpg");
	m_heightMap->Load();

	int max_patch_vertices;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &max_patch_vertices);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	glPointSize(16.0F);
}

void XGLModel::PNTessellation::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glUseProgram(program);
	//Matrixf world = Matrixf::rotate(XGL::Quat(sinf(rad), 0, 0, cosf(rad)));
	Matrixf world = Matrixf::identity();
	glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());

	glUniformMatrix4fv(g_view, 1, GL_FALSE, camera->getInverseMatrix().ptr());

	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_heightmap, 1);
	static float s_tesslevel = 2.0f;
	//s_tesslevel += 0.01f;
	glUniform1f(g_tesslevel, s_tesslevel);

	m_heightMap->Bind(GL_TEXTURE1);
	m_patchMesh->Render();
}

void XGLModel::PNTessellation::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_view = glGetUniformLocation(program, "g_view");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_heightmap = glGetUniformLocation(program, "g_heightmap");
	g_tesslevel = glGetUniformLocation(program, "g_tesslevel");

	//getErrorInformation(GetLastError());
	if (g_world < 0
		|| g_view < 0
		|| g_pers< 0
		|| g_heightmap< 0
		|| g_tesslevel< 0)
	{
		XGLERROR("get uniform failed");
	}
}
