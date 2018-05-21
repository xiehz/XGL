#pragma once

#include "XGL.h"
#include "Vec3"
#include "Matrix"
#include "XCamera.h"
#include "Quat"
#include "XEventAdapter.h"


namespace XGL {



	class XGL_API StandardCamera :public XCamera
	{
		typedef XCamera inherited;

	public:

		// flags
		enum UserInteractionFlags
		{
			UPDATE_MODEL_SIZE = 0x01,
			COMPUTE_HOME_USING_BBOX = 0x02,
			PROCESS_MOUSE_WHEEL = 0x04,
			SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT = 0x08,
			DEFAULT_SETTINGS = UPDATE_MODEL_SIZE /*| COMPUTE_HOME_USING_BBOX*/ | PROCESS_MOUSE_WHEEL
		};

		StandardCamera(int flags = DEFAULT_SETTINGS);
		~StandardCamera();

		/** Sets manipulator by eye position and eye orientation.*/
		virtual void setTransformation(const Vec3f& eye, const Quat& rotation) = 0;

		/** Sets manipulator by eye position, center of rotation, and up vector.*/
		virtual void setTransformation(const Vec3f& eye, const Vec3f& center, const Vec3f& up) = 0;

		/** Gets manipulator's eye position and eye orientation.*/
		virtual void getTransformation(Vec3f& eye, Quat& rotation) const = 0;

		/** Gets manipulator's focal center, eye position, and up vector.*/
		virtual void getTransformation(Vec3f& eye, Vec3f& center, Vec3f& up) const = 0;

		virtual void setVerticalAxisFixed(bool value);
		inline bool getVerticalAxisFixed() const;
		inline bool getAllowThrow() const;
		virtual void setAllowThrow(bool allowThrow);

		virtual void setAnimationTime(const double t);
		double getAnimationTime() const;
		bool isAnimating() const;
		virtual void finishAnimation();

		virtual void init(const XEventAdapter& ea);
		virtual bool handle(const XEventAdapter& ea);

	protected:

		virtual bool handleFrame(const XEventAdapter& ea);
		virtual bool handleResize(const XEventAdapter& ea);
		virtual bool handleMouseMove(const XEventAdapter& ea);
		virtual bool handleMouseDrag(const XEventAdapter& ea);
		virtual bool handleMousePush(const XEventAdapter& ea);
		virtual bool handleMouseRelease(const XEventAdapter& ea);
		virtual bool handleKeyDown(const XEventAdapter& ea);
		virtual bool handleKeyUp(const XEventAdapter& ea);
		virtual bool handleMouseWheel(const XEventAdapter& ea);
		virtual bool handleMouseDeltaMovement(const XEventAdapter& ea);

		virtual bool performMovement();
		virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy);
		virtual bool performMouseDeltaMovement(const float dx, const float dy);
		virtual bool performAnimationMovement(const XEventAdapter& ea);
		virtual void applyAnimationStep(const double currentProgress, const double prevProgress);

		void addMouseEvent(const XEventAdapter& ea);
		void flushMouseEventStack();
		virtual bool isMouseMoving() const;
		float getThrowScale(const double eventTimeDelta) const;
		virtual void centerMousePointer(const XEventAdapter& ea);

		static void rotateYawPitch(Quat& rotation, const double yaw, const double pitch,
			const Vec3f& localUp = Vec3f(0., 0., 0.));


		bool _requestContinousUpdate;
		bool _requestRedraw;
		virtual void requestContinuousUpdate(bool needed = true) { _requestContinousUpdate = needed; }
		virtual void requestRedraw() { _requestRedraw = true; }
		// mouse state
		bool _thrown;
		bool _allowThrow;
		float _mouseCenterX, _mouseCenterY;

		// internal event stack comprising last two mouse events.
		XEventAdapter _ga_t1;
		XEventAdapter _ga_t0;

		/** The approximate amount of time it is currently taking to draw a frame.
		* This is used to compute the delta in translation/rotation during a thrown display update.
		* It allows us to match an delta in position/rotation independent of the rendering frame rate.
		*/
		double _delta_frame_time;

		/** The time the last frame started.
		* Used when _rate_sensitive is true so that we can match display update rate to rotation/translation rate.
		*/
		double _last_frame_time;

		// scene data
		double _modelSize;
		bool _verticalAxisFixed;

		// animation stuff
		class XGL_API AnimationData{ 
		public:
			double _animationTime;
			bool _isAnimating;
			double _startTime;
			double _phase;

			AnimationData();
			virtual void start(const double startTime);
		};
		AnimationData *_animationData;
		virtual void allocAnimationData() { _animationData = new AnimationData(); }

		// flags
		int _flags;

		// flags indicating that a value is relative to model size
		int _relativeFlags;
		inline bool getRelativeFlag(int index) const;
		inline void setRelativeFlag(int index, bool value);
		static int numRelativeFlagsAllocated;
		static int allocateRelativeFlag();

	};


	//
	//  inline methods
	//

	inline bool StandardCamera::getRelativeFlag(int index) const
	{
		return (_relativeFlags & (0x01 << index)) != 0;
	}

	inline void StandardCamera::setRelativeFlag(int index, bool value)
	{
		if (value)  _relativeFlags |= (0x01 << index);
		else  _relativeFlags &= ~(0x01 << index);
	}

	/// Returns whether manipulator preserves camera's "UP" vector.
	inline bool StandardCamera::getVerticalAxisFixed() const
	{
		return _verticalAxisFixed;
	}

	/// Returns true if the camera can be thrown, false otherwise. It defaults to true.
	inline bool StandardCamera::getAllowThrow() const
	{
		return _allowThrow;
	}

}

