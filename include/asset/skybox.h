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

class hdr_skybox : public skybox
{
public:
	hdr_skybox(shared_ptr<texture> a) : tex_ptr(a) {}

	vec3 getBackgroundColor(const ray &r_in) const override
	{
		vec3 dir = normalize(r_in.direction());

		double u = (PI + std::atan(dir.z() / dir.x())) * INV_2PI;
		double v = acos(dir.y()) * INV_PI;

		return tex_ptr->value(u, v, vec3(0));
	}

public:
	shared_ptr<texture> tex_ptr;
};
