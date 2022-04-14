#pragma once

#include "rtweekend.h"
#include "aarect.h"
#include "geometry/hittable_list.h"

class box : public hittable {
public:
    box() {}

    box(const point3 &p0, const point3 &p1, shared_ptr<material> ptr);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

public:
    point3 box_min;
    point3 box_max;
    hittable_list sides;
};