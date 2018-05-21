#include "stdafx.h"
#include "XOrbitCamera.h"
#include "XPlane.h"
using namespace XGL;

int XOrbitCamera::_minimumDistanceFlagIndex = allocateRelativeFlag();

template<class T>
extern void clampBetweenRange(T& value, const T minValue, const T maxValue, const char* valueName);

/// Constructor.
XOrbitCamera::XOrbitCamera(int flags)
	: StandardCamera(flags),
	_distance(1.),
	_trackballSize(0.8)
{
	setMinimumDistance(0.05, true);
	setWheelZoomFactor(0.1);
	if (_flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT)
		setAnimationTime(0.2);
}


/** Set the position of the manipulator using a 4x4 matrix.*/
void XOrbitCamera::setByMatrix(const Matrix& matrix)
{
	_center = Vec3f(0., 0., -_distance) * matrix;
	_rotation = matrix.getRotate();
}


/** Set the position of the manipulator using a 4x4 matrix.*/
void XOrbitCamera::setByInverseMatrix(const Matrix& matrix)
{
	setByMatrix(Matrix::inverse(matrix));
}


/** Get the position of the manipulator as 4x4 matrix.*/
Matrix XOrbitCamera::getMatrix() const
{
	return Matrix::translate(0., 0., _distance) *
		Matrix::rotate(_rotation) *
		Matrix::translate(_center);
}


/** Get the position of the manipulator as a inverse matrix of the manipulator,
typically used as a model view matrix.*/
Matrix XOrbitCamera::getInverseMatrix() const
{
	return Matrix::translate(-_center) *
		Matrix::rotate(_rotation.inverse()) *
		Matrix::translate(0.0, 0.0, -_distance);
}


// doc in parent
void XOrbitCamera::setTransformation(const Vec3f& eye, const Quat& rotation)
{
	_center = eye + rotation * Vec3f(0., 0., -_distance);
	_rotation = rotation;
}


// doc in parent
void XOrbitCamera::getTransformation(Vec3f& eye, Quat& rotation) const
{
	eye = _center - _rotation * Vec3f(0., 0., -_distance);
	rotation = _rotation;
}


// doc in parent
void XOrbitCamera::setTransformation(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
{
	Vec3d lv(center - eye);

	Vec3d f(lv);
	f.normalize();
	Vec3d s(f^up);
	s.normalize();
	Vec3d u(s^f);
	u.normalize();

	Matrix rotation_matrix(s[0], u[0], -f[0], 0.0f,
		s[1], u[1], -f[1], 0.0f,
		s[2], u[2], -f[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	_center = center;
	_distance = lv.length();
	_rotation = rotation_matrix.getRotate().inverse();
}


// doc in parent
void XOrbitCamera::getTransformation(Vec3f& eye, Vec3f& center, Vec3f& up) const
{
	center = _center;
	eye = _center + _rotation * Vec3f(0., 0., _distance);
	up = _rotation * Vec3f(0., 1., 0.);
}


/** Sets the transformation by heading. Heading is given as an angle in radians giving a azimuth in xy plane.
Its meaning is similar to longitude used in cartography and navigation.
Positive number is going to the east direction.*/
void XOrbitCamera::setHeading(double azimuth)
{
	Vec3d localUp = getUpVector();
	Vec3d localRight = getSideVector();

	Vec3d dir = Quat(getElevation(), localRight) * Quat(azimuth, localUp) * Vec3d(0., -_distance, 0.);

	setTransformation(_center + dir, _center, localUp);
}


/// Returns the heading in radians. \sa setHeading
double XOrbitCamera::getHeading() const
{
	Vec3f localFront = getFrontVector();
	Vec3f localRight = getSideVector();

	Vec3f center, eye, tmp;
	getTransformation(eye, center, tmp);

	XPlane frontPlane(localFront, center);
	double frontDist = frontPlane.distance(eye);
	XPlane rightPlane(localRight, center);
	double rightDist = rightPlane.distance(eye);

	return atan2(rightDist, -frontDist);
}


/** Sets the transformation by elevation. Elevation is given as an angle in radians from xy plane.
Its meaning is similar to latitude used in cartography and navigation.
Positive number is going to the north direction, negative to the south.*/
void XOrbitCamera::setElevation(double elevation)
{
	Vec3d localUp = getUpVector();
	Vec3d localRight = getSideVector();

	Vec3d dir = Quat(-elevation, localRight) * Quat(getHeading(), localUp) * Vec3d(0., -_distance, 0.);

	setTransformation(_center + dir, _center, localUp);
}


/// Returns the elevation in radians. \sa setElevation
double XOrbitCamera::getElevation() const
{
	Vec3f localUp = getUpVector();
	localUp.normalize();

	Vec3f center, eye, tmp;
	getTransformation(eye, center, tmp);

	XPlane plane(localUp, center);
	double dist = plane.distance(eye);

	return asin(-dist / (eye - center).length());
}



// doc in parent
bool XOrbitCamera::handleMouseWheel(const XEventAdapter& ea)
{
	XEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	switch (sm)
	{
		// mouse scroll up event
	case XEventAdapter::SCROLL_UP:
	{
		// perform zoom
		zoomModel(_wheelZoomFactor, true);
		//us.requestRedraw();
		//us.requestContinuousUpdate(isAnimating() || _thrown);
		return true;
	}

	// mouse scroll down event
	case XEventAdapter::SCROLL_DOWN:
	{
		// perform zoom
		zoomModel(-_wheelZoomFactor, true);
		//us.requestRedraw();
		//us.requestContinuousUpdate(isAnimating() || _thrown);
		return true;
	}

	// unhandled mouse scrolling motion
	default:
		return false;
	}
}


// doc in parent
bool XOrbitCamera::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// rotate camera
	if (getVerticalAxisFixed()){}
		//rotateWithFixedVertical(dx, dy);
	else
		rotateTrackball(_ga_t0.getXnormalized(), _ga_t0.getYnormalized(),
			_ga_t1.getXnormalized(), _ga_t1.getYnormalized(),
			getThrowScale(eventTimeDelta));
	return true;
}


// doc in parent
bool XOrbitCamera::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// pan model
	float scale = -0.3f * _distance * getThrowScale(eventTimeDelta);
	panModel(dx*scale, dy*scale);
	return true;
}


// doc in parent
bool XOrbitCamera::performMovementRightMouseButton(const double eventTimeDelta, const double /*dx*/, const double dy)
{
	// zoom model
	zoomModel(dy * getThrowScale(eventTimeDelta), true);
	return true;
}


bool XOrbitCamera::performMouseDeltaMovement(const float dx, const float dy)
{
	// rotate camera
	if (getVerticalAxisFixed()){}
		//rotateWithFixedVertical(dx, dy);
	else
		rotateTrackball(0.f, 0.f, dx, dy, 1.f);

	return true;
}


void XOrbitCamera::applyAnimationStep(const double currentProgress, const double prevProgress)
{
	if (_animationData == nullptr) return;

	OrbitAnimationData *ad = dynamic_cast< OrbitAnimationData* >(_animationData);

	// compute new center
	Vec3f prevCenter, prevEye, prevUp;
	getTransformation(prevEye, prevCenter, prevUp);
	Vec3f newCenter = Vec3f(prevCenter) + (ad->_movement * (currentProgress - prevProgress));

	// apply new transformation
	setTransformation(prevEye, newCenter, prevUp);
}



void XOrbitCamera::OrbitAnimationData::start(const Vec3f& movement, const double startTime)
{
	AnimationData::start(startTime);

	_movement = movement;
}


/** Performs trackball rotation based on two points given, for example,
by mouse pointer on the screen.

Scale parameter is useful, for example, when manipulator is thrown.
It scales the amount of rotation based, for example, on the current frame time.*/
void XOrbitCamera::rotateTrackball(const float px0, const float py0,
	const float px1, const float py1, const float scale)
{
	Vec3f axis;
	float angle;

	trackball(axis, angle, px0 + (px1 - px0)*scale, py0 + (py1 - py0)*scale, px0, py0);

	Quat new_rotate;
	new_rotate.makeRotate(angle, axis);

	_rotation = _rotation * new_rotate;
}

/** Moves camera in x,y,z directions given in camera local coordinates.*/
void XOrbitCamera::panModel(const float dx, const float dy, const float dz)
{
	Matrix rotation_matrix;
	rotation_matrix.makeRotate(_rotation);

	Vec3d dv(dx, dy, dz);

	_center += dv * rotation_matrix;
}


/** Changes the distance of camera to the focal center.
If pushForwardIfNeeded is true and minimumDistance is reached,
the focal center is moved forward. Otherwise, distance is limited
to its minimum value.
\sa XOrbitCamera::setMinimumDistance
*/
void XOrbitCamera::zoomModel(const float dy, bool pushForwardIfNeeded)
{
	// scale
	float scale = 1.0f + dy;

	// minimum distance
	float minDist = _minimumDistance;
	if (getRelativeFlag(_minimumDistanceFlagIndex))
		minDist *= _modelSize;

	if (_distance*scale > minDist)
	{
		// regular zoom
		_distance *= scale;
	}
	else
	{
		if (pushForwardIfNeeded)
		{
			// push the camera forward
			float yscale = -_distance;
			Matrixf rotation_matrix(_rotation);
			Vec3d dv = (Vec3d(0.0f, 0.0f, -1.0f) * rotation_matrix) * (dy * yscale);
			_center += dv;
		}
		else
		{
			// set distance on its minimum value
			_distance = minDist;
		}
	}
}


/**
* Simulate a track-ball.  Project the points onto the virtual
* trackball, then figure out the axis of rotation, which is the cross
* product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
* Note:  This is a deformed trackball-- is a trackball in the center,
* but is deformed into a hyperbolic sheet of rotation away from the
* center.  This particular function was chosen after trying out
* several variations.
*
* It is assumed that the arguments to this routine are in the range
* (-1.0 ... 1.0)
*/
void XOrbitCamera::trackball(Vec3f& axis, float& angle, float p1x, float p1y, float p2x, float p2y)
{
	/*
	* First, figure out z-coordinates for projection of P1 and P2 to
	* deformed sphere
	*/

	Matrix rotation_matrix(_rotation);

	Vec3f uv = Vec3d(0.0f, 1.0f, 0.0f)*rotation_matrix;
	Vec3f sv = Vec3d(1.0f, 0.0f, 0.0f)*rotation_matrix;
	Vec3f lv = Vec3d(0.0f, 0.0f, -1.0f)*rotation_matrix;

	Vec3f p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(_trackballSize, p1x, p1y);
	Vec3f p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(_trackballSize, p2x, p2y);

	/*
	*  Now, we want the cross product of P1 and P2
	*/
	axis = p2^p1;
	axis.normalize();

	/*
	*  Figure out how much to rotate around that axis.
	*/
	float t = (p2 - p1).length() / (2.0 * _trackballSize);

	/*
	* Avoid problems with out-of-control values...
	*/
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	angle =XMath::inRadians(asin(t));
}


/**
* Helper trackball method that projects an x,y pair onto a sphere of radius r OR
* a hyperbolic sheet if we are away from the center of the sphere.
*/
float XOrbitCamera::tb_project_to_sphere(float r, float x, float y)
{
	float d, t, z;

	d = sqrt(x*x + y*y);
	/* Inside sphere */
	if (d < r * 0.70710678118654752440)
	{
		z = sqrt(r*r - d*d);
	}                            /* On hyperbola */
	else
	{
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}


/** Set the center of the manipulator. */
void XOrbitCamera::setCenter(const Vec3f& center)
{
	_center = center;
}


/** Get the center of the manipulator. */
const Vec3f& XOrbitCamera::getCenter() const
{
	return _center;
}


/** Set the rotation of the manipulator. */
void XOrbitCamera::setRotation(const Quat& rotation)
{
	_rotation = rotation;
}


/** Get the rotation of the manipulator. */
const Quat& XOrbitCamera::getRotation() const
{
	return _rotation;
}


/** Set the distance of camera to the center. */
void XOrbitCamera::setDistance(double distance)
{
	_distance = distance;
}


/** Get the distance of the camera to the center. */
double XOrbitCamera::getDistance() const
{
	return _distance;
}


/** Set the size of the trackball. Value is relative to the model size. */
void XOrbitCamera::setTrackballSize(const double& size)
{
	/*
	* This size should really be based on the distance from the center of
	* rotation to the point on the object underneath the mouse.  That
	* point would then track the mouse as closely as possible.  This is a
	* simple example, though, so that is left as an Exercise for the
	* Programmer.
	*/
	_trackballSize = size;
	clampBetweenRange(_trackballSize, 0.1, 1.0, "TrackballManipulator::setTrackballSize(float)");
}

template <typename T>
void clampBetweenRange(T& value, const T minValue, const T maxValue, const char* valueName)
{
	if (value<minValue)
	{
		//notify(WARN) << "Warning: " << valueName << " of " << value << " is below permitted minimum, clamping to " << minValue << "." << std::endl;
		value = minValue;
	}
	else
		if (value>maxValue)
		{
			//notify(WARN) << "Warning: " << valueName << " of " << value << " is above permitted maximum, clamping to " << maxValue << "." << std::endl;
			value = maxValue;
		}

}


/** Set the mouse wheel zoom factor.
The amount of camera movement on each mouse wheel event
is computed as the current distance to the center multiplied by this factor.
For example, value of 0.1 will short distance to center by 10% on each wheel up event.
Use negative value for reverse mouse wheel direction.*/
void XOrbitCamera::setWheelZoomFactor(double wheelZoomFactor)
{
	_wheelZoomFactor = wheelZoomFactor;
}


/** Set the minimum distance of the eye point from the center
before the center is pushed forward.*/
void XOrbitCamera::setMinimumDistance(const double& minimumDistance, bool relativeToModelSize)
{
	_minimumDistance = minimumDistance;
	setRelativeFlag(_minimumDistanceFlagIndex, relativeToModelSize);
}


/** Get the minimum distance of the eye point from the center
before the center is pushed forward.*/
double XOrbitCamera::getMinimumDistance(bool *relativeToModelSize) const
{
	if (relativeToModelSize)
		*relativeToModelSize = getRelativeFlag(_minimumDistanceFlagIndex);

	return _minimumDistance;
}
