#include "stdafx.h"
#include "AmbientLight.h"

namespace XGLModel {
	REGISTER(AmbientLight)
}

XGLModel::AmbientLight::AmbientLight()
{
}


XGLModel::AmbientLight::~AmbientLight()
{
}

void XGLModel::AmbientLight::init()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void XGLModel::AmbientLight::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	ModelGL::draw();
}

void XGLModel::AmbientLight::initShader()
{
}
