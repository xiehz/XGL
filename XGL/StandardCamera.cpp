#include "stdafx.h"
#include "StandardCamera.h"

using namespace XGL;

int StandardCamera::numRelativeFlagsAllocated = 0;

int StandardCamera::allocateRelativeFlag()
{
	return numRelativeFlagsAllocated++;
}


/// Constructor.
StandardCamera::StandardCamera(int flags)
	: inherited(),
	_thrown(false),
	_allowThrow(true),
	_mouseCenterX(0.0f), _mouseCenterY(0.0f),
	_delta_frame_time(0.01), _last_frame_time(0.0),
	_modelSize(0.),
	_verticalAxisFixed(false),
	_flags(flags),
	_relativeFlags(0),
	_animationData(nullptr)
{
}

XGL::StandardCamera::~StandardCamera()
{
	if (_animationData != nullptr)
		delete _animationData;
	_animationData = nullptr;
}


/** Makes manipulator to keep camera's "UP" vector.
*
*  In general, fixed up vector makes camera control more user friendly.
*
*  To change up vector, use CameraManipulator::setCoordinateFrameCallback.*/
void StandardCamera::setVerticalAxisFixed(bool value)
{
	_verticalAxisFixed = value;
}


/// Sets manipulator animation time when centering on mouse wheel up is enabled.
void StandardCamera::setAnimationTime(const double t)
{
	if (t <= 0.)
	{
		finishAnimation();
		_animationData = NULL;
		return;
	}

	if (!_animationData)
		allocAnimationData();

	_animationData->_animationTime = t;
}


/// Returns manipulator animation time when centering on mouse wheel up is enabled.
double StandardCamera::getAnimationTime() const
{
	if (_animationData)
		return _animationData->_animationTime;
	else
		return 0.;
}


/// Returns whether manipulator is performing animation at the moment.
bool StandardCamera::isAnimating() const
{
	if (_animationData)
		return _animationData->_isAnimating;
	else
		return false;
}


/// Finishes the animation by performing a step that moves it to its final position.
void StandardCamera::finishAnimation()
{
	_thrown = false;

	if (!isAnimating())
		return;

	applyAnimationStep(1., _animationData->_phase);
}




/** Start/restart the manipulator.*/
void StandardCamera::init(const XEventAdapter& /*ea*/)
{
	flushMouseEventStack();

	// stop animation
	_thrown = false;
}


/** Handles events. Returns true if handled, false otherwise.*/
bool StandardCamera::handle(const XEventAdapter& ea)
{
	switch (ea.getEventType())
	{

	case XEventAdapter::FRAME:
		return handleFrame(ea);

	case XEventAdapter::RESIZE:
		return handleResize(ea);

	default:
		break;
	}

	if (ea.getHandled())
		return false;

	switch (ea.getEventType())
	{
	case XEventAdapter::MOVE:
		return handleMouseMove(ea);

	case XEventAdapter::DRAG:
		return handleMouseDrag(ea);

	case XEventAdapter::PUSH:
		return handleMousePush(ea);

	case XEventAdapter::RELEASE:
		return handleMouseRelease(ea);

	case XEventAdapter::KEYDOWN:
		return handleKeyDown(ea);

	case XEventAdapter::KEYUP:
		return handleKeyUp(ea);

	case XEventAdapter::SCROLL:
		if (_flags & PROCESS_MOUSE_WHEEL)
			return handleMouseWheel(ea);
		else
			return false;

	default:
		return false;
	}
}


/// Handles XEventAdapter::FRAME event.
bool StandardCamera::handleFrame(const XEventAdapter& ea)
{
	double current_frame_time = ea.getTime();

	_delta_frame_time = current_frame_time - _last_frame_time;
	_last_frame_time = current_frame_time;

	if (_thrown && performMovement())
	{
	}

	if (_animationData && _animationData->_isAnimating)
	{
		performAnimationMovement(ea);
	}

	return false;
}

/// Handles XEventAdapter::RESIZE event.
bool StandardCamera::handleResize(const XEventAdapter& ea)
{
	init(ea);
	//us.requestRedraw();

	return true;
}


/// Handles XEventAdapter::MOVE event.
bool StandardCamera::handleMouseMove(const XEventAdapter& /*ea*/)
{
	return false;
}


/// Handles XEventAdapter::DRAG event.
bool StandardCamera::handleMouseDrag(const XEventAdapter& ea)
{
	addMouseEvent(ea);

	if (performMovement()) {

	}
		//us.requestRedraw();

	//us.requestContinuousUpdate(false);
	_thrown = false;

	return true;
}


/// Handles XEventAdapter::PUSH event.
bool StandardCamera::handleMousePush(const XEventAdapter& ea)
{
	flushMouseEventStack();
	addMouseEvent(ea);

	if (performMovement()){}
		//us.requestRedraw();

	//us.requestContinuousUpdate(false);
	_thrown = false;

	return true;
}


/// Handles XEventAdapter::RELEASE event.
bool StandardCamera::handleMouseRelease(const XEventAdapter& ea)
{
	if (ea.getButtonMask() == 0)
	{

		double timeSinceLastRecordEvent = _ga_t0.isValid() ? (ea.getTime() - _ga_t0.getTime()) : DBL_MAX;
		if (timeSinceLastRecordEvent > 0.02)
			flushMouseEventStack();

		if (isMouseMoving())
		{

			if (performMovement() && _allowThrow)
			{
				//us.requestRedraw();
				//us.requestContinuousUpdate(true);
				_thrown = true;
			}

			return true;
		}
	}

	flushMouseEventStack();
	addMouseEvent(ea);
	if (performMovement()){}
		//us.requestRedraw();
	//us.requestContinuousUpdate(false);
	_thrown = false;

	return true;
}


/// Handles XEventAdapter::KEYDOWN event.
bool StandardCamera::handleKeyDown(const XEventAdapter& ea)
{
	//if (ea.getKey() == XEventAdapter::KEY_Space)
	//{
	//	flushMouseEventStack();
	//	_thrown = false;
	//	home(ea, us);
	//	return true;
	//}

	return false;
}


/// Handles XEventAdapter::KEYUP event.
bool StandardCamera::handleKeyUp(const XEventAdapter& /*ea*/)
{
	return false;
}


/// Handles XEventAdapter::SCROLL event.
bool StandardCamera::handleMouseWheel(const XEventAdapter& /*ea*/)
{
	return false;
}



/// Make movement step of manipulator. Returns true if any movement was made.
bool StandardCamera::performMovement()
{
	// return if less then two events have been added
	if (_ga_t0.isValid()== false|| _ga_t1.isValid()== false)
		return false;

	// get delta time
	double eventTimeDelta = _ga_t0.getTime() - _ga_t1.getTime();
	if (eventTimeDelta < 0.)
	{
		//OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
		eventTimeDelta = 0.;
	}

	// get deltaX and deltaY
	float dx = _ga_t0.getXnormalized() - _ga_t1.getXnormalized();
	float dy = _ga_t0.getYnormalized() - _ga_t1.getYnormalized();

	// return if there is no movement.
	if (dx == 0. && dy == 0.)
		return false;


	// call appropriate methods
	unsigned int buttonMask = _ga_t1.getButtonMask();
	//unsigned int modKeyMask = _ga_t1.getModKeyMask();
	if (buttonMask == XEventAdapter::LEFT_MOUSE_BUTTON)
	{
		return performMovementLeftMouseButton(eventTimeDelta, dx, dy);
	}
	else if ((buttonMask == XEventAdapter::MIDDLE_MOUSE_BUTTON) ||
		(buttonMask == (XEventAdapter::LEFT_MOUSE_BUTTON | XEventAdapter::RIGHT_MOUSE_BUTTON)))
	{
		return performMovementMiddleMouseButton(eventTimeDelta, dx, dy);
	}
	else if (buttonMask == XEventAdapter::RIGHT_MOUSE_BUTTON)
	{
		return performMovementRightMouseButton(eventTimeDelta, dx, dy);
	}

	return false;
}


/** Make movement step of manipulator.
This method implements movement for left mouse button.*/
bool StandardCamera::performMovementLeftMouseButton(const double /*eventTimeDelta*/, const double /*dx*/, const double /*dy*/)
{
	return false;
}


/** Make movement step of manipulator.
This method implements movement for middle mouse button
or combination of left and right mouse button pressed together.*/
bool StandardCamera::performMovementMiddleMouseButton(const double /*eventTimeDelta*/, const double /*dx*/, const double /*dy*/)
{
	return false;
}


/** Make movement step of manipulator.
This method implements movement for right mouse button.*/
bool StandardCamera::performMovementRightMouseButton(const double /*eventTimeDelta*/, const double /*dx*/, const double /*dy*/)
{
	return false;
}


/// The method processes events for manipulation based on relative mouse movement (mouse delta).
bool StandardCamera::handleMouseDeltaMovement(const XEventAdapter& ea)
{
	float dx = ea.getX() - _mouseCenterX;
	float dy = ea.getY() - _mouseCenterY;

	if (dx == 0.f && dy == 0.f)
		return false;

	addMouseEvent(ea);
	centerMousePointer(ea);

	return performMouseDeltaMovement(dx, dy);
}


/// The method performs manipulator update based on relative mouse movement (mouse delta).
bool StandardCamera::performMouseDeltaMovement(const float /*dx*/, const float /*dy*/)
{
	return false;
}


/// Makes the manipulator progress in its current animation.
bool StandardCamera::performAnimationMovement(const XEventAdapter& ea)
{
	double f = (ea.getTime() - _animationData->_startTime) / _animationData->_animationTime;
	if (f >= 1.)
	{
		f = 1.;
		_animationData->_isAnimating = false;
		if (!_thrown){}
			//us.requestContinuousUpdate(false);
	}

	applyAnimationStep(f, _animationData->_phase);

	_animationData->_phase = f;
	//us.requestRedraw();

	return _animationData->_isAnimating;
}


/// Updates manipulator by a single animation step
void StandardCamera::applyAnimationStep(const double /*currentProgress*/, const double /*prevProgress*/)
{
}


/// Centers mouse pointer
void StandardCamera::centerMousePointer(const XEventAdapter& ea)
{
	_mouseCenterX = (ea.getXmin() + ea.getXmax()) / 2.0f;
	_mouseCenterY = (ea.getYmin() + ea.getYmax()) / 2.0f;
	//us.requestWarpPointer(_mouseCenterX, _mouseCenterY);
}


/** Add the current mouse GUIEvent to internal stack.*/
void StandardCamera::addMouseEvent(const XEventAdapter& ea)
{
	_ga_t1 = _ga_t0;
	_ga_t0 = ea;
}


/** Reset the internal GUIEvent stack.*/
void StandardCamera::flushMouseEventStack()
{
	_ga_t1.setValid(false);
	_ga_t0.setValid(false);
}


/** Check the speed at which the mouse is moving.
If speed is below a threshold then return false, otherwise return true.*/
bool StandardCamera::isMouseMoving() const
{
	if (_ga_t0.isValid() == false|| _ga_t1.isValid()== false) return false;

	static const float velocity = 0.1f;

	float dx = _ga_t0.getXnormalized() - _ga_t1.getXnormalized();
	float dy = _ga_t0.getYnormalized() - _ga_t1.getYnormalized();
	float len = sqrtf(dx*dx + dy*dy);
	float dt = _ga_t0.getTime() - _ga_t1.getTime();

	return (len>dt*velocity);
}


/** Set the 'allow throw' flag. If it is set to true (default), releasing the mouse button
while moving the mouse results in a throw. If manipulator was thrown, it continues spinning
although no mouse button is down at the moment.*/
void StandardCamera::setAllowThrow(bool allowThrow)
{
	_allowThrow = allowThrow;
}


/** Returns the scale that should be applied on animation of "thrown" manipulator state
to avoid its dependency on varying frame rate.

eventTimeDelta parameter gives the time difference between last two
events that started the animation.*/
float StandardCamera::getThrowScale(const double eventTimeDelta) const
{
	if (_thrown)
	{
		if (eventTimeDelta == 0.f)
			return 0.f;
		return float(_delta_frame_time / eventTimeDelta);
	}
	else  return 1.f;
}


/** Update rotation by yaw and pitch.
*
*  localUp parameter defines either camera's "UP" vector
*  that will be preserved during rotation, or it can be zero (0,0,0) to specify
*  that camera's "UP" vector will be not preserved and free rotation will be made.*/
void StandardCamera::rotateYawPitch(Quat& rotation, const double yaw, const double pitch,
	const Vec3f& localUp)
{
	bool verticalAxisFixed = (localUp != Vec3d(0., 0., 0.));

	// rotations
	Quat rotateYaw(-yaw, verticalAxisFixed ? localUp : rotation * Vec3d(0., 1., 0.));
	Quat rotatePitch;
	Quat newRotation;
	Vec3d cameraRight(rotation * Vec3d(1., 0., 0.));

	double my_dy = pitch;
	int i = 0;

	do {

		// rotations
		rotatePitch.makeRotate(my_dy, cameraRight);
		newRotation = rotation * rotateYaw * rotatePitch;

		// check for viewer's up vector to be more than 90 degrees from "up" axis
		Vec3d newCameraUp = newRotation * Vec3d(0., 1., 0.);
		if (newCameraUp * localUp > 0.)
		{

			// apply new rotation
			rotation = newRotation;
			return;

		}

		my_dy /= 2.;
		if (++i == 20)
		{
			rotation = rotation * rotateYaw;
			return;
		}

	} while (true);
}

StandardCamera::AnimationData::AnimationData() :
	_animationTime(0.0),
	_isAnimating(false),
	_startTime(0.0),
	_phase(0.0)
{
}


void StandardCamera::AnimationData::start(const double startTime)
{
	_isAnimating = true;
	_startTime = startTime;
	_phase = 0.;
}
