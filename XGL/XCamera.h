#pragma once
#include "XGL.h"
#include "Vec3f"
#include "Matrix"
#include "XEventHandler.h"
#include "XEventAdapter.h"

namespace XGL {

	class XGL_API XCamera :public XEventHandler
	{
	public:
		XCamera();
		virtual ~XCamera();
	public:
		Vec3f getSideVector()const { return Vec3f(1.0f, 0.0f, 0.0f); }
		Vec3f getFrontVector()const { return Vec3f(0.0f, 0.0f, 1.0f); }
		Vec3f getUpVector()const { return Vec3f(0.0f, 1.0f, 0.0f); }

		virtual void setByMatrix(const Matrix& matrix) = 0;

		/** set the position of the matrix manipulator using a 4x4 Matrix.*/
		virtual void setByInverseMatrix(const Matrix& matrix) = 0;

		/** get the position of the manipulator as 4x4 Matrix.*/
		virtual Matrix getMatrix() const = 0;

		/** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
		virtual Matrix getInverseMatrix() const = 0;

	public:
		/** Manually set the home position, and set the automatic compute of home position. */
		virtual void setHomePosition(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
		{
			_homeEye = eye;
			_homeCenter = center;
			_homeUp = up;
		}

		/** Get the manually set home position. */
		virtual void getHomePosition(Vec3f& eye, Vec3f& center, Vec3f& up) const
		{
			eye = _homeEye;
			center = _homeCenter;
			up = _homeUp;
		}

		/** finish any active manipulator animations.*/
		virtual void finishAnimation() {}

		/**
		Move the camera to the default position.
		May be ignored by manipulators if home functionality is not appropriate.
		*/
		virtual void home(const XEventAdapter&) {}

		/**
		Move the camera to the default position.
		This version does not require GUIEventAdapter and GUIActionAdapter so may be
		called from somewhere other than a handle() method in GUIEventHandler.  Application
		must be aware of implications.
		*/
		virtual void home(double /*currentTime*/) {}

		/**
		Start/restart the manipulator.
		FIXME: what does this actually mean? Provide examples.
		*/
		virtual void init(const XEventAdapter&) {}

	protected:
		Vec3f _homeEye;
		Vec3f _homeCenter;
		Vec3f _homeUp;
	};


}

