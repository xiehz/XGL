#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"
#include "CubeShadowMap.h"


XGLModel::XCameraDirection g_cameraInfos[6] = {
{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, XGL::Vec3f(1.0f, 0.0f, 0.0f),  XGL::Vec3f(0.0f, 1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, XGL::Vec3f(-1.0f, 0.0f, 0.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, XGL::Vec3f(0.0f, 1.0f, 0.0f),  XGL::Vec3f(-1.0f, 0.0f, .0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, XGL::Vec3f(0.0f, -1.0f, 0.0f), XGL::Vec3f(1.0f, 0.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, XGL::Vec3f(0.0f, 0.0f, 1.0f),  XGL::Vec3f(0.0f, 1.0f, 0.0f) },
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
	g_world = glGetUniformLocation(program, "g_world");
	g_vp = glGetUniformLocation(program, "g_vp");
	g_eyeworld = glGetUniformLocation(program, "g_eyeworld");

	if (g_world < 0
		|| g_vp< 0
		|| g_eyeworld < 0)
	{
		XGLERROR("get uniform failed");
	}
}

void XGLModel::CubeShadowMap::setupPers(XGL::Vec3f eye, int face)
{
	//90透视投影，刚好看向6个方向
	static float fov = 90.0f * 3.1415926f / 180.0f;
	float tanha = tanf(fov / 2);
	float aspect = (float)300 / (float)300;
	float nearclip = 1.0f, farclip = 10000.0f;

	double right = tanha * aspect * nearclip;
	double left = -right;
	double top = tanha * nearclip;
	double bottom = -top;
	ModelGL::project(left, right, bottom, top, nearclip, farclip);

	XOrbitCamera lightview;
	lightview.setTransformation(eye, g_cameraInfos[face].target, g_cameraInfos[face].up);
	Matrixf view = lightview.getInverseMatrix();
	view.postMult(projectMatrix);

	glUniformMatrix4fv(g_vp, 1, GL_FALSE , view.ptr());

	glUniform3f(g_eyeworld, eye.x(), eye.y(), eye.z());
}


void XGLModel::CubeShadowMap::setWorld(const XGL::Matrixf& world)
{
	glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
}

