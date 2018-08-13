#include "stdafx.h"
#include "xgl\XOrbitCamera.h"
#include "xgl\Error.h"
#include "IXMesh.h"
#include "AxesShape.h"
#include "SphereShape.h"
#include "DepthImage.h"
#include "CubeShadowMap.h"

XGLModel::XCameraDirection g_cameraInfos[6] = 
{
{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, XGL::Vec3f(1.0f, 0.0f, 0.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, XGL::Vec3f(-1.0f, 0.0f, 0.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, XGL::Vec3f(0.0f, 1.0f, 0.0f), XGL::Vec3f(0.0f, 0.0f, 1.0f) },
{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, XGL::Vec3f(0.0f, -1.0f, 0.0f), XGL::Vec3f(0.0f, 0.0f, -1.0f) },
{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, XGL::Vec3f(0.0f, 0.0f, 1.0f), XGL::Vec3f(0.0f, -1.0f, 0.0f) },
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

XGL::Matrixf XGLModel::CubeShadowMap::calcLightView(const XGL::Vec3f& eye, const XGL::Vec3f& look, const XGL::Vec3f& up0) {

	XGL::Vec3f forward = -look;
	forward.normalize();

	XGL::Vec3f right = up0 ^ forward;

	if (right == XGL::Vec3f())
		right = XGL::Vec3f(1.0F, 0.0F, 0.0F);

	XGL::Vec3f up = forward ^ right;

	float m[4][4] = {
		right.x(), up.x(), forward.x(), 0,
		right.y(), up.y(), forward.y(), 0,
		right.z(), up.z(), forward.z(), 0,
		-eye * right, -eye * up, -eye * forward, 1,
	};

	return Matrixf(*m);
}

void XGLModel::CubeShadowMap::setupPers(XGL::Vec3f eye, int face)
{
	//90透视投影，刚好看向6个方向
	static float fov = 90.0f * 3.1415926f / 180.0f;
	float tanha = tanf(fov / 2);
	float aspect = (float)512 / (float)512;
	float nearclip = 1.0f, farclip = 10000.0f;

	double right = tanha * aspect * nearclip;
	double left = -right;
	double top = tanha * nearclip;
	double bottom = -top;
	ModelGL::project(left, right, bottom, top, nearclip, farclip);

	Matrixf view = calcLightView(eye, g_cameraInfos[face].forward, g_cameraInfos[face].up);
	view.postMult(projectMatrix);

	glUniformMatrix4fv(g_vp, 1, GL_FALSE , view.ptr());

	glUniform3f(g_eyeworld, eye.x(), eye.y(), eye.z());
}


void XGLModel::CubeShadowMap::setWorld(const XGL::Matrixf& world)
{
	glUniformMatrix4fv(g_world, 1, GL_FALSE, world.ptr());
}

