#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	// cylinder centered on the z axis
	class cylinder : public object
	{
	public:
		cylinder() = default;
		cylinder(double radius, double phi_max, double zmin, double zmax, shared_ptr<material> m);
		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;

		double get_area() const {
			return (zmax - zmin) * radius * phi_max;
		}

	public:
		double radius, phi_max, zmin, zmax;
		shared_ptr<material> mat_ptr;
	};
}