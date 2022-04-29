#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	// paraboloid centered on the z axis with radius r and height h
	class paraboloid : public object
	{
	public:
		paraboloid(double radius, double zmin, double zmax, double phi_max, shared_ptr<material> m);
		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

		double get_area() const
		{
			double radius2 = radius * radius;
			double k = 4 * zmax / radius2;
			return (radius2 * radius2 * phi_max / (12 * zmax * zmax))
				* (std::pow(k * zmax + 1, 1.5) - std::pow(k * zmin + 1, 1.5));
		}
	public:
		double radius, zmin, zmax, phi_max;
		shared_ptr<material> mat_ptr;
	};
}