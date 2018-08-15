#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "SSAOCore.h"


XGLModel::SSAOCore::SSAOCore()
{
}


XGLModel::SSAOCore::~SSAOCore()
{
}

void XGLModel::SSAOCore::initGL()
{
}

void XGLModel::SSAOCore::draw()
{

}

void XGLModel::SSAOCore::initUniform()
{
	g_posmap = glGetUniformLocation(program, "g_posmap");
	g_samplerrad = glGetUniformLocation(program, "g_samplerrad");

	g_pers = glGetUniformLocation(program, "g_pers");
	g_kernels = glGetUniformLocation(program, "g_kernels");

	//getErrorInformation(GetLastError());
	if (g_posmap < 0
		|| g_samplerrad < 0
		|| g_pers < 0
		|| g_kernels < 0)
	{
		XGLERROR("get uniform failed");
	}
}
void XGLModel::SSAOCore::GenKernel()
{
	Vector3f kernel[KERNEL_SIZE];

	for (unsigned int  i = 0; i < KERNEL_SIZE; i++) {
		float scale = (float)i / (float)(KERNEL_SIZE);
		Vector3f v;
		v.x = 2.0f * (float)rand() / RAND_MAX - 1.0f;
		v.y = 2.0f * (float)rand() / RAND_MAX - 1.0f;
		v.z = 2.0f * (float)rand() / RAND_MAX - 1.0f;
		// Use an acceleration function so more points are
		// located closer to the origin
		v *= (0.1f + 0.9f * scale * scale);

		kernel[i] = v;
	}

	glUniform3fv(g_kernels, KERNEL_SIZE, (const GLfloat*)&kernel[0]);
}

