#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	class hyperboloid : public object
	{
	public:
		hyperboloid(point3 point1, point3 point2, double phi, shared_ptr<material> m);
		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

	public:
		point3 p1, p2;
		double zmin, zmax;
		double phi_max;
		double r_max;
		double ah, ch;

		shared_ptr<material> mat_ptr;
	};
}