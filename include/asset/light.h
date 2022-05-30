#pragma once

#include "material.h"

class diffuse_light : public material
{
public:
	diffuse_light(shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	color emitted(const ray& r_in, const hit_record& rec,
		double u, double v, const point3& p) const override
	{
		if (rec.front_face)
			return emit->value(u, v, p);
		return color(0, 0, 0);
	}

public:
	shared_ptr<texture> emit;
};

class spot_light : public material
{
public:
	spot_light(color c, vec3 dir, double cosTotalWidth_, double cosFalloffStart_) :
		emit(make_shared<solid_color>(c)), spot_direction(normalize(dir)), cosTotalWidth(cosTotalWidth_), cosFalloffStart(cosFalloffStart_)
	{
	}

	spot_light(shared_ptr<texture> a, vec3 dir, double cosTotalWidth_, double cosFalloffStart_) :
		emit(a), spot_direction(normalize(dir)), cosTotalWidth(cosTotalWidth_), cosFalloffStart(cosFalloffStart_)
	{
	}

	color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override
	{
		if (rec.front_face)
			return(falloff(r_in.origin() - rec.p) * emit->value(u, v, p));
		return color(0, 0, 0);
	}

	double spot_light::falloff(const vec3& w) const
	{
		double cosTheta = dot(spot_direction, normalize(w));
		if (cosTheta < cosTotalWidth) {
			return(0);
		}
		if (cosTheta > cosFalloffStart) {
			return(1);
		}
		double delta = (cosTheta - cosTotalWidth) / (cosFalloffStart - cosTotalWidth);
		return((delta * delta) * (delta * delta));
	}
public:
	shared_ptr<texture> emit;
	vec3 spot_direction;
	const double cosTotalWidth, cosFalloffStart;
};