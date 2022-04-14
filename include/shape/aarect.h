#pragma once

#include "geometry/hittable.h"
#include "rtweekend.h"

class xy_rect : public hittable {
public:
    xy_rect() {}

    xy_rect(double x0_, double x1_, double y0_, double y1_, double k_, shared_ptr<material> mat)
            : x0(x0_), x1(x1_), y0(y0_), y1(y1_), k(k_), mp(mat) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.

        output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }

public:
    shared_ptr<material> mp;
    // z = k
    double x0{}, x1{}, y0{}, y1{}, k{};
};

class xz_rect : public hittable {
public:
    xz_rect() {}

    xz_rect(double x0_, double x1_, double z0_, double z1_, double k_, shared_ptr<material> mat)
            : x0(x0_), x1(x1_), z0(z0_), z1(z1_), k(k_), mp(mat) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }
	double pdf_value(const point3 &o, const vec3 &v) const override;
	vec3 random(const vec3 &o) const override;

public:
    shared_ptr<material> mp;
    double x0, x1, z0, z1, k;
};

class yz_rect : public hittable {
public:
    yz_rect() {}

    yz_rect(double y0_, double y1_, double z0_, double z1_, double k_, shared_ptr<material> mat)
            : y0(y0_), y1(y1_), z0(z0_), z1(z1_), k(k_), mp(mat) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

public:
    shared_ptr<material> mp;
    double y0, y1, z0, z1, k;
};
