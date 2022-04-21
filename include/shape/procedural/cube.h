#pragma once

#include "geometry/hittable.h"

namespace shape::procedural
{
	class cube : public hittable
	{
	public:
		cube() {}
		cube(point3 cen, double length, shared_ptr<material> m);

		bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
	public:
		point3 center;
		double side_length;
		shared_ptr<material> mat_ptr;
	};
}

