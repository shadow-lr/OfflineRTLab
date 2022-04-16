#pragma once

#include "ray.h"
#include "..\rtweekend.h"

class material;
class aabb;

struct hit_record
{
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	// u v coordinates in surface
	double u, v;
	bool front_face;

	// if r.dir are not in a hemisphere with outward_normal，then front_face = false
	inline void set_face_normal(const ray &r, const vec3 &outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// This class(hittable) represents any object that can be hit by ray
class hittable
{
public:
	// ray intersection detection
	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

	virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0;
	// probability density function
	virtual double pdf_value(const point3 &origin, const vec3 &v) const { return 0.0; }

	virtual vec3 random(const vec3 &origin) const { return vec3(1, 0, 0); }

	virtual ~hittable() {}
};

// make normals point in the −y direction
class flip_face : public hittable
{
public:
	flip_face(shared_ptr<hittable> p) : ptr(p) {}
private:
	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
	{
		if (!ptr->hit(r, t_min, t_max, rec))
			return false;

		rec.front_face = !rec.front_face;
		return true;
	}
	bool bounding_box(double time0, double time1, aabb &output_box) const override
	{
		return ptr->bounding_box(time0, time1, output_box);
	}
public:
	shared_ptr<hittable> ptr;
};
