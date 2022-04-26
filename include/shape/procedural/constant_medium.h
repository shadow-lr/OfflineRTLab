#pragma once

#include "geometry/hittable_list.h"

class texture;

namespace shape::procedural
{
	class constant_medium : public hittable_list
	{
	public:
		constant_medium(shared_ptr<object> b, double d, shared_ptr<texture> a);
		constant_medium(shared_ptr<object> b, double d, color c);

		virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

		virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
		{
			return boundary->bounding_box(time0, time1, output_box);
		}

	public:
		shared_ptr<object> boundary;
		shared_ptr<material> phase_function;
		double neg_inv_density;
	};
}

