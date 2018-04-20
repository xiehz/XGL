#include "stdafx.h"
#include "Orbit.h"
#include "OrbitCamera.h"

using namespace XGL;
const float ANGLE_SCALE = 0.2f;
const float ZOOM_SCALE = 0.5f;
const float MIN_DIST = 1.0f;
const float MAX_DIST = 3.0f;

OrbitCamera::OrbitCamera()
{
	this->oribit = new Orbit();
}


OrbitCamera::~OrbitCamera()
{
	delete this->oribit;
	this->oribit = nullptr;
}

const Matrix4 & XGL::OrbitCamera::getViewMatrix()
{
	viewMatrix = this->oribit->getMatrix();
	return Camera::getViewMatrix();
}

void XGL::OrbitCamera::lookat(const Vector3 & eye, const Vector3 & center, const Vector3 & up)
{
	this->oribit->lookAt(eye, center, up);
}

void XGL::OrbitCamera::project(float l, float r, float b, float t, float n, float f)
{
	this->projectionMatrix.identity();
	projectionMatrix[0] = 2 * n / (r - l);
	projectionMatrix[5] = 2 * n / (t - b);
	projectionMatrix[8] = (r + l) / (r - l);
	projectionMatrix[9] = (t + b) / (t - b);
	projectionMatrix[10] = -(f + n) / (f - n);
	projectionMatrix[11] = -1;
	projectionMatrix[14] = -(2 * f * n) / (f - n);
	projectionMatrix[15] = 0;
}

int XGL::OrbitCamera::lButtonDown(WPARAM state, int x, int y)
{
	mouse_X = x;
	mouse_Y = y;
	leftButton_Down = true;
	return 0;
}

int XGL::OrbitCamera::lButtonUp(WPARAM state, int x, int y)
{
	mouse_X = x;
	mouse_Y = y;
	leftButton_Down = false;
	return 0;
}

int XGL::OrbitCamera::rButtonDown(WPARAM state, int x, int y)
{
	mouse_X = x;
	mouse_Y = y;
	rightButton_Down = true;
	return 0;
}

int XGL::OrbitCamera::rButtonUp(WPARAM state, int x, int y)
{
	mouse_X = x;
	mouse_Y = y;
	rightButton_Down = false;
	return 0;
}

int XGL::OrbitCamera::mouseMove(WPARAM state, int x, int y)
{
	if (leftButton_Down)
	{
		Vector3 angle = this->oribit->getAngle();
		angle.y -= (x - mouse_X) * ANGLE_SCALE;
		angle.x += (y - mouse_Y) * ANGLE_SCALE;

		// constrain x angle -89 < x < 89
		if (angle.x < -89.0f)
			angle.x = -89.0f;
		else if (angle.x > 89.0f)
			angle.x = 89.0f;

		this->oribit->rotateTo(angle);
	}
	else if (rightButton_Down){

	}
	else {

	}
	
	mouse_X = x;
	mouse_Y = y;
	return 0;
}

int XGL::OrbitCamera::mouseWheel(int state, int delta, int x, int y)
{
	float d = delta/1200.0f;
	float distance = this->oribit->getDistance();
	distance -= (d * ZOOM_SCALE);

	//// constrain min and max
	//if (distance < MIN_DIST)
	//	distance = MIN_DIST;
	//else if (distance > MAX_DIST)
	//	distance = MAX_DIST;

	this->oribit->setDistance(distance);
	return 0;
}

