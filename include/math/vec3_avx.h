#pragma once

#include <immintrin.h>

#include <cmath>
#include <iostream>

#include "rtweekend.h"

using std::sqrt;

class vec2
{
public:
	vec2() : e(_mm_set1_pd(0)) {}
	vec2(double e0, double e1) : e(_mm_setr_pd(e0, e1)) {}
	vec2(double e0) : e(_mm_set_pd(e0, e0)) {}

	vec2(const __m128d& e_) : e(e_) {}

	double x() const { return e.m128d_f64[0]; }
	double y() const { return e.m128d_f64[1]; }

	vec2 operator-() const { return vec2(_mm_sub_pd(_mm_setzero_pd(), e)); }
	double operator[](int i) const { return e.m128d_f64[i]; }
	double &operator[](int i) { return e.m128d_f64[i]; }

	bool operator==(const vec2 &other) const {return e.m128d_f64[0] == other[0] && e.m128d_f64[1] == other[1];}

	vec2& operator+=(const vec2& v) { e = _mm_add_pd(e, v.e); return *this; }
	vec2& operator-=(const vec2& v) { e = _mm_sub_pd(e, v.e); return *this; }
	vec2& operator*=(const vec2& v) { e = _mm_mul_pd(e, v.e); return *this; }
	vec2& operator/=(const vec2& v) { e = _mm_div_pd(e, v.e); return *this; }

	vec2& operator+=(const double t) { *this += vec2(t); return *this; }
	vec2& operator-=(const double t) { *this -= vec2(t); return *this; }
	vec2& operator*=(const double t) { *this *= vec2(t); return *this; }
	vec2& operator/=(const double t) { return *this *= (1.0 / t); }

	double length() const { return sqrt(length_squared()); }

	double length_squared() const
	{
		const __m128d e2 = _mm_mul_pd(e, e);
		return e2.m128d_f64[0] + e2.m128d_f64[1];
	}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e.m128d_f64[0]) < s) && (fabs(e.m128d_f64[1]) < s);
	}
public:
	__m128d e;
};

class vec3
{
public:
	vec3() : e(_mm256_set1_pd(0)) {}
	vec3(double e0, double e1, double e2) : e(_mm256_setr_pd(e0, e1, e2, 0)) {}
	vec3(double e0) : e(_mm256_set1_pd(e0)) {}
	vec3(const __m256d& e_) : e(e_) {}

	double x() const { return e.m256d_f64[0]; }
	double y() const { return e.m256d_f64[1]; }
	double z() const { return e.m256d_f64[2]; }

	vec3 operator-() const { return vec3(_mm256_sub_pd(_mm256_setzero_pd(), e)); }
	double operator[](int i) const { return e.m256d_f64[i]; }
	double &operator[](int i) { return e.m256d_f64[i]; }

	vec3& operator+=(const vec3& v) { e = _mm256_add_pd(e, v.e); return *this; }
	vec3& operator-=(const vec3& v) { e = _mm256_sub_pd(e, v.e); return *this; }
	vec3& operator*=(const vec3& v) { e = _mm256_mul_pd(e, v.e); return *this; }
	vec3& operator/=(const vec3& v) { e = _mm256_div_pd(e, v.e); return *this; }

	vec3& operator+=(const double t) { e = _mm256_add_pd(e, _mm256_set1_pd(t)); return *this; }
	vec3& operator-=(const double t) { e = _mm256_sub_pd(e, _mm256_set1_pd(t)); return *this; }
	vec3& operator*=(const double t) { e = _mm256_mul_pd(e, _mm256_set1_pd(t)); return *this; }
	vec3& operator/=(const double t) { e = _mm256_mul_pd(e, _mm256_set1_pd(1.0 / t)); return *this; }

	double length() const { return sqrt(length_squared()); }

	double length_squared() const
	{
		const __m256d e2 = _mm256_mul_pd(e, e);
		return e2.m256d_f64[0] + e2.m256d_f64[1] + e2.m256d_f64[2];
	}

	inline static vec3 random() { return vec3(random_double(), random_double(), random_double()); }

	inline static vec3 random(double min, double max) {return vec3(random_double(min, max), random_double(min, max), random_double(min, max));}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e.m256d_f64[0]) < s) && (fabs(e.m256d_f64[1]) < s) && (fabs(e.m256d_f64[2]) < s);
	}
public:
	__m256d e;
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
// x(r), y(g), z(b)
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
	return out << v.e.m256d_f64[0] << ' ' << v.e.m256d_f64[1] << ' ' << v.e.m256d_f64[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(_mm256_add_pd(u.e, v.e)); }
inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(_mm256_sub_pd(u.e, v.e)); }
inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(_mm256_mul_pd(u.e, v.e)); }
inline vec3 operator/(const vec3 &u, const vec3 &v) { return vec3(_mm256_div_pd(u.e, v.e)); }

inline vec3 operator+(double t, const vec3 &v) { return vec3(_mm256_add_pd(_mm256_set1_pd(t), v.e)); }
inline vec3 operator-(double t, const vec3 &v) { return vec3(_mm256_sub_pd(_mm256_set1_pd(t), v.e)); }
inline vec3 operator*(double t, const vec3 &v) { return vec3(_mm256_mul_pd(_mm256_set1_pd(t), v.e)); }
inline vec3 operator/(double t, const vec3 &v) { return vec3(_mm256_mul_pd(_mm256_set1_pd(1.0 / t), v.e)); }

inline vec3 operator+(const vec3 &v, double t) { return vec3(_mm256_add_pd(v.e, _mm256_set1_pd(t))); }
inline vec3 operator-(const vec3 &v, double t) { return vec3(_mm256_sub_pd(v.e, _mm256_set1_pd(t))); }
inline vec3 operator*(const vec3 &v, double t) { return vec3(_mm256_mul_pd(v.e, _mm256_set1_pd(t))); }
inline vec3 operator/(const vec3 &v, double t) { return vec3(_mm256_mul_pd(v.e, _mm256_set1_pd(1.0 / t))); }

inline double dot(const vec3 &u, const vec3 &v)
{
	const __m256d c = _mm256_mul_pd(u.e, v.e);
	return c.m256d_f64[0] + c.m256d_f64[1] + c.m256d_f64[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
	const __m256d a012 = u.e;
	const __m256d b012 = v.e;
	const __m256d a201 = _mm256_permute4x64_pd(a012, _MM_SHUFFLE(3, 1, 0, 2));
	const __m256d b201 = _mm256_permute4x64_pd(b012, _MM_SHUFFLE(3, 1, 0, 2));
	__m256d tmp = _mm256_fmsub_pd(b012, a201, _mm256_mul_pd(a012, b201));
	tmp = _mm256_permute4x64_pd(tmp, _MM_SHUFFLE(3, 1, 0, 2));
	tmp = _mm256_blend_pd(_mm256_setzero_pd(), tmp, 0x7); // put zero on 4th position
	return vec3(tmp);
}

inline double Max(const vec3& v) { return std::max(v.x(), std::max(v.y(), v.z())); }
inline double Min(const vec3& v) { return std::min(v.x(), std::min(v.y(), v.z())); }

inline vec3 Max(const vec3& l, const vec3& r) { return vec3(_mm256_max_pd(l.e, r.e)); }
inline vec3 Min(const vec3& l, const vec3& r) { return vec3(_mm256_min_pd(l.e, r.e)); }

// Get normlized vector
inline vec3 normalize(vec3 v)
{
	return vec3(_mm256_div_pd(v.e, _mm256_set1_pd(v.length())));
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
