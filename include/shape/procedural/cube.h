#pragma once

#include "geometry/object.h"
#include "square.h"

#include <vector>

namespace shape::procedural
{
	class cube : public object
	{
	public:
		cube(point3 cen, double length, shared_ptr<material> m);

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
	public:
		point3 center;
		double side_length;
		// six square
		std::vector<square> squares;
		shared_ptr<material> mat_ptr;
	};
}

