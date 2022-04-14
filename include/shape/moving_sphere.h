#pragma once

#include "rtweekend.h"

#include "geometry/hittable.h"
#include "geometry/aabb.h"

class moving_sphere : public hittable {
public:
	moving_sphere() = default;

	moving_sphere(point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	// 根据time获得此时的center球心
	point3 center(double time) const;

	// 正方体AABB包围盒 包围 球
	virtual bool bounding_box(double _time0, double _time1, aabb& output_box) const {
		aabb box0(center(_time0) - vec3(radius, radius, radius), center(_time0) + vec3(radius, radius, radius));
		aabb box1(center(_time1) - vec3(radius, radius, radius), center(_time1) + vec3(radius, radius, radius));
		output_box = surrounding_box(box0, box1);
		return true;
	}

public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	std::shared_ptr<material> mat_ptr;
};
