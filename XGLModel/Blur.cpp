#include "stdafx.h"
#include "Blur.h"


XGLModel::Blur::Blur()
{
}


XGLModel::Blur::~Blur()
{
}

void XGLModel::Blur::initGL()
{
}

void XGLModel::Blur::draw()
{

}

void XGLModel::Blur::initUniform()
{
	g_aomap = glGetUniformLocation(program, "g_aomap");
	//getErrorInformation(GetLastError());
	if (g_aomap < 0)
	{
		XGLERROR("get uniform failed");
	}
}

