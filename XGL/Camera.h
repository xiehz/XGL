#pragma once
#include <vector>
#include "XGL.h"
#include "Matrices.h"
namespace XGL {
	class XGL_API Camera
	{
	public:
		Camera() { leftButton_Down = false; rightButton_Down = false; }
		virtual ~Camera() {}
		virtual const Matrix4& getViewMatrix() { return viewMatrix; }
		virtual const Matrix4& getProjectMatrix() { return projectionMatrix; };
		virtual void project(float left, float right, float bottom,float top, float near, float far) = 0;
		virtual void lookat(const Vector3& eye, const Vector3& center, const Vector3& up) = 0;

	public:
		virtual  int lButtonDown(WPARAM state, int x, int y) = 0;
		virtual  int lButtonUp(WPARAM state, int x, int y) = 0;
		virtual  int rButtonDown(WPARAM state, int x, int y) = 0;
		virtual  int rButtonUp(WPARAM state, int x, int y) = 0;
		virtual  int mouseMove(WPARAM state, int x, int y) = 0;
		virtual  int mouseWheel(int state, int delta, int x, int y) = 0; 
	public:
		virtual const std::vector<Vector3>& getMousePath() { return sphere_Path; }
		virtual const Vector3& getMouseVector() { return sphere_Vector; };
	protected:
		Vector3 sphere_Vector;
		std::vector<Vector3> sphere_Path;
		Matrix4 projectionMatrix;
		Matrix4 viewMatrix;
	protected:
		bool leftButton_Down;
		bool rightButton_Down;
		int mouse_X;
		int mouse_Y;
	};
}


