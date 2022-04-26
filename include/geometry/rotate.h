#pragma once

#include "hittable_list.h"
#include "geometry/aabb.h"

class rotate_y : public hittable_list {
public:
    rotate_y(shared_ptr<object> p, double angle);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
	{
		output_box = bbox;
		return hasbox;
	}

public:
    shared_ptr<object> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
};