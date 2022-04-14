#pragma once
#include "geometry/hittable.h"

class cube : public hittable {
public:
    cube() {}

    cube(point3 cen, double length, shared_ptr<material> m) : center(cen), side_length(length), mat_ptr(m) {};

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

public:
    point3 center;
    double side_length;
    shared_ptr<material> mat_ptr;
};

bool cube::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {

    return true;
}

