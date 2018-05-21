#pragma once
#include "XGL.h"
#include "Vec3f"

namespace XGL {
	class XGL_API XPlane
	{
	public:
		XPlane();
		virtual ~XPlane();
		inline XPlane(const Vec3f& norm, const Vec3f& point) { set(norm, point); }
		inline void set(const Vec3f& norm, const Vec3f& point)
		{
			double d = -norm[0] * point[0] - norm[1] * point[1] - norm[2] * point[2];
			set(norm[0], norm[1], norm[2], d);
		}
		inline void set(double a, double b, double c, double d)
		{ _fv[0] = a; _fv[1] = b; _fv[2] = c; _fv[3] = d; }

		inline double distance(const Vec3f& v) const
		{
			return _fv[0] * v.x() +
				_fv[1] * v.y() +
				_fv[2] * v.z() +
				_fv[3];
		}

	protected:
		double _fv[4];
	};
}
