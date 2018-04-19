#include "stdafx.h"
#include "Vectors.h"
#include "animUtils.h"
#include "Trackball.h"
#include "Quaternion.h"
#include "TrackballCamera.h"

using namespace XGL;

TrackballCamera::TrackballCamera()
{
	this->leftButton_Down = false;
	this->camera_Distance = 0.f;
	this->trackball = new Trackball(1,1,1);
	this->trackball->setMode(Trackball::PROJECT);
	this->quat = new Quaternion(1, 0, 0, 0);
}


TrackballCamera::~TrackballCamera()
{
	delete this->trackball;
	this->trackball = nullptr;
	delete this->quat;
	this->quat = nullptr;
}

void XGL::TrackballCamera::home(float radius, int s_width, int s_height)
{
	this->camera_Distance = radius*2 ;
	this->trackball->setRadius(radius);
	this->trackball->setScreenSize(s_width, s_height);
}

const Matrix4 & XGL::TrackballCamera::getViewMatrix()
{
	viewMatrix = this->quat->getMatrix().transpose().translate(0, 0, -camera_Distance);
	return Camera::getViewMatrix();
}



void XGL::TrackballCamera::lookat(const Vector3 & eye, const Vector3 & center, const Vector3 & up)
{
}

void XGL::TrackballCamera::project(float l, float r, float b, float t, float n, float f)
{
	projectionMatrix.identity();
	projectionMatrix[0] = 2 * n / (r - l);
	projectionMatrix[5] = 2 * n / (t - b);
	projectionMatrix[8] = (r + l) / (r - l);
	projectionMatrix[9] = (t + b) / (t - b);
	projectionMatrix[10] = -(f + n) / (f - n);
	projectionMatrix[11] = -1;
	projectionMatrix[14] = -(2 * f * n) / (f - n);
	projectionMatrix[15] = 0;
}


int XGL::TrackballCamera::lButtonDown(WPARAM state, int x, int y)
{
	this->leftButton_Down = true;
	this->mouse_X = x;
	this->mouse_Y = y;

	return 0;
}

int XGL::TrackballCamera::lButtonUp(WPARAM state, int x, int y)
{
	this->leftButton_Down = false;
	this->sphere_Path.clear();
	return 0;
}

int XGL::TrackballCamera::rButtonDown(WPARAM state, int x, int y)
{
	return 0;
}

int XGL::TrackballCamera::rButtonUp(WPARAM state, int x, int y)
{
	return 0;
}

int XGL::TrackballCamera::mouseMove(WPARAM state, int x, int y)
{
	if (leftButton_Down)
	{
		Vector3 pre_radius_v = this->trackball->getUnitVector(this->mouse_X, this->mouse_Y);
		Vector3 cur_radius_v = this->trackball->getUnitVector(x, y);

		Quaternion dq = Quaternion::getQuaternion(pre_radius_v, cur_radius_v);

		dq = dq* (*this->quat);
		
		this->quat->set(dq.s, dq.x, dq.y, dq.z);
		generateMousePath();
	}
	else {
		this->sphere_Vector = this->trackball->getVector(x, y);
	}

	this->mouse_X = x;
	this->mouse_Y = y;
	return 0;
}

int XGL::TrackballCamera::mouseWheel(int state, int delta, int x, int y)
{
	this->camera_Distance += delta / 120.0f;
	return 0;
}

void XGL::TrackballCamera::generateMousePath()
{
	Vector3 v1 = sphere_Vector;
	Vector3 v2 = trackball->getVector(mouse_X, mouse_Y);
	float alpha;

	sphere_Path.clear();
	for (int i = 0; i <= 30; ++i)
	{
		alpha = (float)i / 30;
		Vector3 v = slerp(v1, v2, alpha);
		v = v.normalize() * (trackball->getRadius() + 0.3f);
		sphere_Path.push_back(v);
	}

	/*
	Vector3 v1 = sphereVector;
	Vector3 v2 = trackball.getVector(mouseX, mouseY);
	Vector3 v;
	float alpha;

	pathPoints.clear();
	for(int i = 0; i <= PATH_COUNT; ++i)
	{
	alpha = (float)i / PATH_COUNT;
	v = v1 + (v2 - v1) * alpha;
	v = v.normalize() * sphereRadius;
	pathPoints.push_back(v);
	}
	*/
}


