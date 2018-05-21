#pragma once
#include "XGL.h"
#include "Vec3f"
#include "Quat"
#include "Matrix"
#include "StandardCamera.h"
#include "XEventAdapter.h"

namespace XGL {


	/** OrbitManipulator is base class for camera control based on focal center,
	distance from the center, and orientation of distance vector to the eye.
	This is the base class for trackball style manipulators.*/
	class XGL_API XOrbitCamera : public StandardCamera
	{

	public:

		XOrbitCamera(int flags = DEFAULT_SETTINGS);

		virtual void setByMatrix(const Matrix& matrix);
		virtual void setByInverseMatrix(const Matrix& matrix);
		virtual Matrix getMatrix() const;
		virtual Matrix getInverseMatrix() const;

		virtual void setTransformation(const Vec3f& eye, const Quat& rotation);
		virtual void setTransformation(const Vec3f& eye, const Vec3f& center, const Vec3f& up);
		virtual void getTransformation(Vec3f& eye, Quat& rotation) const;
		virtual void getTransformation(Vec3f& eye, Vec3f& center, Vec3f& up) const;

		void setHeading(double azimuth);
		double getHeading() const;
		void setElevation(double elevation);
		double getElevation() const;

		virtual void setCenter(const Vec3f& center);
		const Vec3f& getCenter() const;
		virtual void setRotation(const Quat& rotation);
		const Quat& getRotation() const;
		virtual void setDistance(double distance);
		double getDistance() const;

		virtual void setTrackballSize(const double& size);
		inline double getTrackballSize() const;
		virtual void setWheelZoomFactor(double wheelZoomFactor);
		inline double getWheelZoomFactor() const;

		virtual void setMinimumDistance(const double& minimumDistance, bool relativeToModelSize = false);
		double getMinimumDistance(bool *relativeToModelSize = NULL) const;

	protected:

		virtual bool handleMouseWheel(const XEventAdapter& ea);

		virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMouseDeltaMovement(const float dx, const float dy);
		virtual void applyAnimationStep(const double currentProgress, const double prevProgress);

		virtual void rotateTrackball(const float px0, const float py0,
			const float px1, const float py1, const float scale);
		virtual void panModel(const float dx, const float dy, const float dz = 0.f);
		virtual void zoomModel(const float dy, bool pushForwardIfNeeded = true);
		void trackball(Vec3f& axis, float& angle, float p1x, float p1y, float p2x, float p2y);
		float tb_project_to_sphere(float r, float x, float y);

		Vec3f _center;
		Quat  _rotation;
		double     _distance;

		double _trackballSize;
		double _wheelZoomFactor;

		double _minimumDistance;
		static int _minimumDistanceFlagIndex;

		class XGL_API  OrbitAnimationData : public AnimationData {
		public:
			Vec3f _movement;
			virtual void start(const Vec3f& movement, const double startTime);
		};
		virtual void allocAnimationData() { _animationData = new OrbitAnimationData(); }
	};


	//
	//  inline functions
	//

	/** Get the size of the trackball relative to the model size. */
	inline double XOrbitCamera::getTrackballSize() const { return _trackballSize; }
	/** Get the mouse wheel zoom factor.*/
	inline double XOrbitCamera::getWheelZoomFactor() const { return _wheelZoomFactor; }

}


