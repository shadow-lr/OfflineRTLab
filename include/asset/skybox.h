#pragma once

#include "math/vec3.h"
#include "geometry/ray.h"

#include "material.h"
#include "texture.h"
#include "shape/procedural/square.h"

using shape::procedural::square;

class skybox
{
public:
	virtual vec3 getBackgroundColor(const ray &r_in) const = 0;
	virtual ~skybox() {}
};

class constant_skybox : public skybox
{
public:
	constant_skybox(color c = color(0, 0, 0)) : backgroundColor(c) {}
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

class hdr_skybox : public skybox
{
public:
	hdr_skybox(shared_ptr<texture> a) : tex_ptr(a) {}

	vec3 getBackgroundColor(const ray &r_in) const override
	{
		vec3 dir = normalize(r_in.direction());

		double theta = acos(dir.y());
		double phi = atan2(dir.z(), dir.x()) + PI;

		double u = phi * INV_2PI;
		double v = theta * INV_PI;

		const static vec3 zero = vec3(0);

		return tex_ptr->value(u, v, zero);
	}

public:
	shared_ptr<texture> tex_ptr;
};
