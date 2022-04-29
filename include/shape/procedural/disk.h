#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	//  a Disk is a circular disk of radius(r) at height(h) along the z axis
	class disk : public object
	{
	public:
		disk(double height, double radius, double inner_radius, double phi_max, shared_ptr<material> m);
		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

		double get_area() const
		{
			return phi_max * 0.5 * (radius * radius - inner_radius * inner_radius);
		}
	public:
		double height, radius, inner_radius, phi_max;
		shared_ptr<material> mat_ptr;
	};
}