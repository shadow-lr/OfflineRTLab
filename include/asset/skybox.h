#pragma once

#include "math/vec3.h"
#include "geometry/ray.h"

class skybox
{
public:
	virtual vec3 getBackgroundColor(const ray &r_in) const = 0;
	virtual ~skybox() {}
};

class constant_skybox : public skybox
{
public:
	constant_skybox(const color &c) : backgroundColor(c) {}
	vec3 getBackgroundColor(const ray &r_in) const override
	{
		return backgroundColor;
	}

	color backgroundColor;
};

class blue_skybox : public skybox
{
public:
	vec3 getBackgroundColor(const ray &r_in) const override
	{
		vec3 unit_direction = normalize(r_in.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
};

