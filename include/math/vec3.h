#pragma once

#include <cmath>
#include <iostream>

#include "rtweekend.h"

using std::sqrt;

class vec2
{
	vec2() : e{0, 0} {}
	vec2(double e0, double e1) : e{e0, e1} {}
	vec2(double e0) : e{e0, e0} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }

	vec2 operator-() const { return vec2(-e[0], -e[1]); }
	double operator[](int i) const { return e[i]; }
	double &operator[](int i) { return e[i]; }

	vec2& operator+=(const vec2& v) { e[0] += v.e[0]; e[1] += v.e[1]; return *this; }
	vec2& operator-=(const vec2& v) { e[0] -= v.e[0]; e[1] -= v.e[1]; return *this; }
	vec2& operator*=(const vec2& v) { e[0] *= v.e[0]; e[1] *= v.e[1]; return *this; }
	vec2& operator/=(const vec2& v) { e[0] /= v.e[0]; e[1] /= v.e[1]; return *this; }

	vec2& operator+=(const double t) { e[0] += t; e[1] += t; return *this; }
	vec2& operator-=(const double t) { e[0] -= t; e[1] -= t; return *this; }
	vec2& operator*=(const double t) { e[0] *= t; e[1] *= t; return *this; }
	vec2& operator/=(const double t) { return *this *= 1 / t; }

	double length() const { return sqrt(length_squared()); }

	double length_squared() const { return e[0] * e[0] + e[1] * e[1]; }

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s);
	}

public:
	double e[2];
};

class vec3
{
public:
	vec3() : e{0, 0, 0} {}
	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
	vec3(double e0) : e{e0, e0, e0} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3& v) { e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2]; return *this; }
	vec3& operator-=(const vec3& v) { e[0] -= v.e[0]; e[1] -= v.e[1]; e[2] -= v.e[2]; return *this; }
	vec3& operator*=(const vec3& v) { e[0] *= v.e[0]; e[1] *= v.e[1]; e[2] *= v.e[2]; return *this; }
	vec3& operator/=(const vec3& v) { e[0] /= v.e[0]; e[1] /= v.e[1]; e[2] /= v.e[2]; return *this; }

	vec3& operator+=(const double t) { e[0] += t; e[1] += t; e[2] += t; return *this; }
	vec3& operator-=(const double t) { e[0] -= t; e[1] -= t; e[2] -= t; return *this; }
	vec3& operator*=(const double t) { e[0] *= t; e[1] *= t; e[2] *= t; return *this; }
	vec3& operator/=(const double t) { return *this *= 1 / t; }

	double length() const { return sqrt(length_squared()); }

	double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	inline static vec3 random() { return vec3(random_double(), random_double(), random_double()); }

	inline static vec3 random(double min, double max) {return vec3(random_double(min, max), random_double(min, max), random_double(min, max));}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

public:
	double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
// x(r), y(g), z(b)
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(u) += v; }
inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(u) -= v; }
inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(u) *= v; }
inline vec3 operator/(const vec3 &u, const vec3 &v) { return vec3(u) /= v; }

inline vec3 operator+(double t, const vec3 &v) { return vec3(t) + v; }
inline vec3 operator-(double t, const vec3 &v) { return vec3(t) - v; }
inline vec3 operator*(double t, const vec3 &v) { return vec3(t) * v; }
inline vec3 operator/(double t, const vec3 &v) { return vec3(t) / v; }

inline vec3 operator+(const vec3 &v, double t) { return vec3(v) += t; }
inline vec3 operator-(const vec3 &v, double t) { return vec3(v) -= t; }
inline vec3 operator*(const vec3 &v, double t) { return vec3(v) *= t; }
inline vec3 operator/(const vec3 &v, double t) { return vec3(v) *= (1.0 / t); }

inline double dot(const vec3 &u, const vec3 &v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline double Max(const vec3& v) { return std::max(v.x(), std::max(v.y(), v.z())); }
inline double Min(const vec3& v) { return std::min(v.x(), std::min(v.y(), v.z())); }

inline vec3 Max(const vec3& l, const vec3& r) { return { std::max(l.x(), r.x()), std::max(l.y(), r.y()), std::max(l.z(), r.z()) }; }
inline vec3 Min(const vec3& l, const vec3& r) { return { std::min(l.x(), r.x()), std::min(l.y(), r.y()), std::min(l.z(), r.z()) }; }

// Get normlized vector
inline vec3 normalize(vec3 v)
{
	return v / v.length();
}

// Randomly select a point from the unit sphere
static vec3 random_in_unit_sphere()
{
	auto[r1, r2] = random_point2d();
	double theta = TWO_PI * r1;
	double phi = std::acos(2 * r2 - 1);

	return vec3(std::sin(theta) * std::sin(phi), std::cos(theta) * std::sin(phi), std::cos(phi));
}

// reject
static vec3 random_in_unit_sphere_reject()
{
	while (true)
	{
		vec3 p = vec3::random(-1, 1);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

inline vec3 random_to_sphere(double radius, double distance_squared) {
	auto r1 = random_double();
	auto r2 = random_double();
	auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

	auto phi = TWO_PI * r1;
	auto x = cos(phi) * sqrt(1 - z * z);
	auto y = sin(phi) * sqrt(1 - z * z);

	return vec3(x, y, z);
}

static vec3 random_unit_vector()
{
	return normalize(random_in_unit_sphere());
}

// Randomly select a point from the hemisphere
static vec3 random_in_hemisphere(const vec3 &normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();

	// In the same hemisphere as the normal
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

static vec3 random_cosine_direction()
{
	auto[r1, r2] = random_point2d();
	auto z = sqrt(1 - r2);

	auto phi = 2 * PI * r1;
	auto x = std::cos(phi) * std::sqrt(r2);
	auto y = std::sin(phi) * std::sqrt(r2);

	return vec3(x, y, z);
}

// Randomly select a point from the disk
static vec3 random_in_unit_disk()
{
	auto [r1, r2] = random_point2d();

	double theta = r1;
	double phi = r2 * TWO_PI;

	return vec3(theta * std::cos(phi), theta * std::sin(phi), 0);
}

// reject
static vec3 random_in_unit_disk_reject()
{
	while (true)
	{
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

// reflect equation
static vec3 reflect(const vec3 &v, const vec3 &n)
{
	return v - 2 * dot(v, n) * n;
}

// refract equation
static vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

