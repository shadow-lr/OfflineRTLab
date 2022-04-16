#pragma once

#include "geometry/hittable.h"
#include "math/vec3.h"

namespace shape::model
{
	class triangle : public hittable
	{
	public:
		triangle(vec3 v0_, vec3 v1_, vec3 v2_, shared_ptr<material> ptr = nullptr)
			: v0(v0_), v1(v1_), v2(v2_), mat_ptr(ptr)
		{
			e1 = v1 - v0;
			e2 = v2 - v0;
			normal = normalize(cross(e1, e2));
			area = cross(e1, e2).length() * 0.5;
		}

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

		~triangle();

		double get_area() const { return area; }

	public:
		vec3 v0, v1, v2; // vertices A, B ,C , counter-clockwise order
		vec3 e1, e2;     // 2 edges v1-v0, v2-v0;
		vec3 t0, t1, t2; // texture coords
		vec3 normal;
		double area;

		shared_ptr<material> mat_ptr;
	};
}