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

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline std::pair<double, double> random_point2d() {
    return std::make_pair(random_double(), random_double());
}

// return a random value in the interval between min value and max value
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

// Common Headers
#include "geometry/ray.h"
#include "math/vec3.h"

