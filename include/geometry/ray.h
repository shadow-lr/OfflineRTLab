#pragma once

#include "math/vec3.h"

class ray {
public:
    ray() = default;

    ray(const point3 &origin, const vec3 &direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time) {}

    point3 origin() const { return orig; }

    vec3 direction() const { return dir; }

    double time() const { return tm; }

    point3 at(double t) const {
        return orig + t * dir;
    }

public:
    // A + tb
    // A
    point3 orig;
    // d
    vec3 dir;

    double tm;
};

