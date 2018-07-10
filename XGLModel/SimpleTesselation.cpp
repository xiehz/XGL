#include "stdafx.h"
#include "xgl\Math"
#include "SimpleTesselation.h"

namespace XGLModel {
	REGISTER(SimpleTesselation)
}
const float rad = -45.f * 3.1415926f / 360.f;

XGLModel::SimpleTesselation::SimpleTesselation()
{
}


XGLModel::SimpleTesselation::~SimpleTesselation()
{
}

void XGLModel::SimpleTesselation::initGL()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT, GL_LINE);

	int max_patch_vertices;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &max_patch_vertices);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

}

void XGLModel::SimpleTesselation::draw()
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
	glDrawArrays(GL_PATCHES, 0, 6);
}

void XGLModel::SimpleTesselation::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_eye = glGetUniformLocation(program, "g_eye");
	g_view = glGetUniformLocation(program, "g_view");
	g_pers = glGetUniformLocation(program, "g_pers");
	//getErrorInformation(GetLastError());
	if (g_world < 0||
		g_eye < 0
		|| g_view < 0
		|| g_pers< 0)
	{
		XGLERROR("get uniform failed");
	}
}
