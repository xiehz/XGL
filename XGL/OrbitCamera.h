#pragma once
#include "XGL.h"
#include "Camera.h"
#include "Matrices.h"

namespace XGL {

	class Orbit;
	class XGL_API OrbitCamera :public Camera
	{
	public:
		OrbitCamera();
		virtual ~OrbitCamera();

	public:
		virtual const Matrix4& getViewMatrix() override;
		// Í¨¹ý Camera ¼Ì³Ð
		virtual void lookat(const Vector3& eye, const Vector3& center, const Vector3& up) override;
		virtual void project(float left, float right, float bottom, float top, float near, float) override;
		
		virtual int lButtonDown(WPARAM state, int x, int y) override;
		virtual int lButtonUp(WPARAM state, int x, int y) override;
		virtual int rButtonDown(WPARAM state, int x, int y) override;
		virtual int rButtonUp(WPARAM state, int x, int y) override;
		virtual int mouseMove(WPARAM state, int x, int y) override;
		virtual int mouseWheel(int state, int delta, int x, int y) override;


	private:
		XGL::Orbit* oribit;
	};
}


