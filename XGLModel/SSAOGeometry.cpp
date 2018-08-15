#include "stdafx.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "SSAOGeometry.h"


XGLModel::SSAOGeometry::SSAOGeometry()
{

}


XGLModel::SSAOGeometry::~SSAOGeometry()
{
}

void XGLModel::SSAOGeometry::initGL()
{

}

void XGLModel::SSAOGeometry::draw()
{

}

void XGLModel::SSAOGeometry::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_mvp = glGetUniformLocation(program, "g_mvp");
	//getErrorInformation(GetLastError());
	if (g_mv < 0|| g_mvp < 0)
	{
		XGLERROR("get uniform failed");
	}
}

