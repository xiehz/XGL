#include "stdafx.h"
#include "xgl\Math"
#include "XTexture.h"
#include "XPatchMesh.h"
#include "BasicTesselation.h"

namespace XGLModel {
	REGISTER(BasicTesselation)
}
const float rad = -45.f * 3.1415926f / 360.f;

XGLModel::BasicTesselation::BasicTesselation()
{
	m_patchMesh = 0;
	m_heightMap = 0;
}


XGLModel::BasicTesselation::~BasicTesselation()
{
	if (m_patchMesh)
		delete m_patchMesh;
	m_patchMesh = 0;
	if (m_heightMap)
		delete m_heightMap;
	m_heightMap = 0;
}

void XGLModel::BasicTesselation::initGL()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT, GL_LINE);

	m_patchMesh = new XPatchMesh();
	m_patchMesh->LoadMesh("E:/2018/opengl/Assimp/data/quad.obj");

	m_heightMap = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/heightmap.jpg");
	m_heightMap->Load();

	int max_patch_vertices;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &max_patch_vertices);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

}

void XGLModel::BasicTesselation::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glUseProgram(program);
	//Matrixf world = Matrixf::rotate(XGL::Quat(sinf(rad), 0, 0, cosf(rad)));
	Matrixf world = Matrixf::identity();
	glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());

	glUniformMatrix4fv(g_view, 1, GL_FALSE, camera->getInverseMatrix().ptr());
	XGL::Vec3f eye = camera->getMatrix().getTrans();
	glUniform3f(g_eye, eye.x(), eye.y(), eye.z());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_heightmap, 1);

	m_heightMap->Bind(GL_TEXTURE1);
	m_patchMesh->Render();
}

void XGLModel::BasicTesselation::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_eye = glGetUniformLocation(program, "g_eye");
	g_view = glGetUniformLocation(program, "g_view");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_heightmap = glGetUniformLocation(program, "g_heightmap");
	//getErrorInformation(GetLastError());
	if (g_world < 0||
		g_eye < 0
		|| g_view < 0
		|| g_pers< 0
		|| g_heightmap< 0)
	{
		XGLERROR("get uniform failed");
	}
}
