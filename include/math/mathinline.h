#pragma once

#include "./rtweekend.h"

inline double CosTheta(const vec3 &w) { return w.z(); }
inline double Cos2Theta(const vec3 &w) { return w.z() * w.z(); }
inline double AbsCosTheta(const vec3 &w) { return std::abs(w.z()); }

inline double Sin2Theta(const vec3 &w) { return std::fmax(0, 1 - Cos2Theta(w)); }
inline double SinTheta(const vec3 &w) { return std::sqrt(Sin2Theta(w)); }

inline double TanTheta(const vec3 &w) { return SinTheta(w) / CosTheta(w); }
inline double Tan2Theta(const vec3 &w) { return Sin2Theta(w) / Cos2Theta(w); }

inline double CosPhi(const vec3 &w)
{
	double sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 1 : clamp(w.x() / sinTheta, -1, 1);
}

inline double SinPhi(const vec3& w)
{
	double sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 0 : clamp(w.y() / sinTheta, -1, 1);
}

inline double Cos2Phi(const vec3& w)
{
	return CosPhi(w) * CosPhi(w);
}

inline double Sin2Phi(const vec3& w)
{
	return SinPhi(w) * SinPhi(w);
}

// the fast inverse square root implementation from Quake III Arena,
static double Q_rsqrt(double number)
{
	long i;
	double x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}