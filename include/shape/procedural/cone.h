#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	// cone centered on the z axis with radius z and height h
	class cone : public object
	{
	public:
		cone(double height, double radius, double phi_max, shared_ptr<material> m);

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

		double get_area() const
		{
			return radius * std::sqrt((height * height) + (radius * radius)) * phi_max / 2;
		}

	public:
		double height, radius, phi_max;
		shared_ptr<material> mat_ptr;
	};
}