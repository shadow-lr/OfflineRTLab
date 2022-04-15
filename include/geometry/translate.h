#pragma once

#include "hittable_list.h"

class translate : public hittable_list
{
public:
	translate(shared_ptr<hittable> p, const vec3 &displacement)
		: ptr(p), offset(displacement) {}

	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
	shared_ptr<hittable> ptr;
	vec3 offset;
};