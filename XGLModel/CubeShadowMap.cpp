#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"
#include "CubeShadowMap.h"


XGLModel::XCameraDirection g_cameraInfos[6] = {
{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, XGL::Vec3f(1.0f, 0.0f, 0.0f),  XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, XGL::Vec3f(-1.0f, 0.0f, 0.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, XGL::Vec3f(0.0f, 1.0f, 0.0f),  XGL::Vec3f(0.0f, 0.0f, -1.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, XGL::Vec3f(0.0f, -1.0f, 0.0f), XGL::Vec3f(0.0f, 0.0f, 1.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, XGL::Vec3f(0.0f, 0.0f, 1.0f),  XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, XGL::Vec3f(0.0f, 0.0f, -1.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) }
};

XGLModel::CubeShadowMap::CubeShadowMap()
{
}


XGLModel::CubeShadowMap::~CubeShadowMap()
{
}

void XGLModel::CubeShadowMap::draw()
{

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

}

void XGLModel::CubeShadowMap::initGL()
{

}

void XGLModel::CubeShadowMap::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");

	if (g_mv < 0
		|| g_pers< 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::CubeShadowMap::setupPers()
{
	//90透视投影，刚好看向6个方向
	static float fov = 90.0f * 3.1415926f / 180.0f;
	float tanha = tanf(fov / 2);
	float aspect = (float)windowWith / (float)windowHeight;
	float nearclip = 1.0f, farclip = 10000.0f;

	double right = tanha * aspect * nearclip;
	double left = -right;
	double top = tanha * nearclip;
	double bottom = -top;
	ModelGL::project(left, right, bottom, top, nearclip, farclip);

	glUniformMatrix4fv(g_pers, 1, GL_FALSE , projectMatrix.ptr());

}

void XGLModel::CubeShadowMap::setWorld(XGL::Vec3f eye, int face, const XGL::Matrixf& world)
{
	XOrbitCamera lightview;
	lightview.setTransformation(eye, g_cameraInfos[face].target, g_cameraInfos[face].up);
	Matrixf view = lightview.getInverseMatrix();

	view.preMult(world);
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, view.ptr());
}

