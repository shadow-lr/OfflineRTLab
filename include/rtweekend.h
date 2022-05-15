#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <cstdlib>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();

#define PI        3.14159265358979323846
#define TWO_PI    6.28318530717958647692
#define INV_PI    0.31830988618379067154
#define INV_2PI   0.15915494309189533577

#define EPSILON 0.00016

// Utility Functions

inline double degrees_to_radians(double degrees)
{
	return degrees * PI / 180.0;
}

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline std::pair<double, double> random_point2d()
{
	return std::make_pair(random_double(), random_double());
}

// return a random value in the interval between min value and max value
inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

template<typename T, typename U, typename R>
inline T clamp(T x, U min, R max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline int random_int(int min, int max)
{
	return static_cast<int>(random_double(min, max + 1));
}

inline bool quadratic(double a, double b, double c, double& t0, double& t1)
{
	double discrim = b * b - 4 * a * c;
	// equation has no solution
	if (discrim < 0)
		return false;
	double root_discrim = std::sqrt(discrim);

	double q;
	if (b < 0)
	{
		q = -0.5 * (b - root_discrim);
	}
	else
	{
		q = -0.5 * (b + root_discrim);
	}
	t0 = q / a;
	t1 = c / q;
	if (t0 > t1)
		std::swap(t0, t1);

	return true;
}
