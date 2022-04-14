#pragma once

#include "rtweekend.h"

class onb
{
public:
	onb() = default;

	inline vec3 operator[](int i) const { return axis[i]; }
	vec3 u() const { return axis[0]; }
	vec3 v() const { return axis[1]; }
	vec3 w() const { return axis[2]; }

	vec3 local(double a, double b, double c) const
	{
		return a * u() + b * v() + c * w();
	}

	vec3 local(const vec3 &a) const
	{
		return a.x() * u() + a.y() * v() + a.z() * w();
	}

	// n is w axis
	void build_from_w(const vec3 &n)
	{
		build_from_w_normal(n);
//		build_from_w_hughes_moller(n);
//		build_from_w_frisvad(n);
	}

	void build_from_w_normal(const vec3& n)
	{
		axis[2] = unit_vector(n);
		vec3 a = (std::fabs(w().x()) > 0.999) ? vec3(0, 1, 0) : vec3(1, 0, 0);
		// n a
		axis[1] = unit_vector(cross(w(), a));
		axis[0] = cross(w(), v());
	}

	// Real-Time Rendering 4.2.4
	// Hughes and Möller 1999
	void build_from_w_hughes_moller(const vec3 &n)
	{
		axis[2] = unit_vector(n);
		if (std::abs(w().x()) > std::abs(w().z()))
		{
			axis[1] = unit_vector(vec3(-w().y(), w().x(), 0.0));
			double length = std::sqrt(1.0 - w().z() * w().z());
			double r = 1.0 / length;
			axis[1] = vec3(-w().y() * r, w().x() * r, 0.0);
		}
		else
		{
			axis[1] = unit_vector(vec3(0.0, -w().z(), w().y()));
			double length = std::sqrt(1.0 - w().x() * w().x());
			double r = 1.0 / length;
			axis[1] = vec3(0.0, -w().z() * r, w().y() * r);
		}
		// left hand
		// axis[0] = cross(w(), axis[1]);

		// right hand
		axis[0] = cross(axis[1], w());
	}

	// Jeppe Revall Frisvad 2012
	void build_from_w_frisvad(const vec3& n)
	{
		axis[2] = unit_vector(n);
		if (w().z() < -0.9999999f)
		{
			axis[0] = vec3(0.0, -1.0, 0.0);
			axis[1] = vec3(-1.0, 0.0, 0.0);
			return;
		}

		double a = -1.0 / (1.0 + w().z());
		double b = w().x() * w().y() * a;

		axis[0] = vec3(1.0 + w().x() * w().x() * a, b, -w().x());
		axis[1] = vec3(b, 1.0 + w().y() * w().y() * a, -w().y());
	}

public:
	// o n b(t b n)
	vec3 axis[3];
};
