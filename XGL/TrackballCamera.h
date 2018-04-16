#pragma once
#include "Camera.h"

namespace XGL
{
	class Trackball;
	struct Quaternion;
	struct Vector3;
	class XGL_API TrackballCamera:public Camera
	{
	public:
		TrackballCamera();
		virtual ~TrackballCamera();

		void home(float radius, int s_width, int s_height);
		virtual const Matrix4& getViewMatrix() override;
		virtual void lookat(const Vector3& eye, const Vector3& center, const Vector3& up) ;
		virtual void project(float left, float right, float bottom, float top, float near, float far);
		virtual void generateMousePath();


	public:
		virtual  int lButtonDown(WPARAM state, int x, int y);
		virtual  int lButtonUp(WPARAM state, int x, int y);
		virtual  int rButtonDown(WPARAM state, int x, int y);
		virtual  int rButtonUp(WPARAM state, int x, int y);
		virtual  int mouseMove(WPARAM state, int x, int y);
		virtual  int mouseWheel(int state, int delta, int x, int y);
	private:
		Trackball* trackball;
		Quaternion* quat;
		float camera_Distance;
	};
}


