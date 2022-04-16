#pragma once

#include "geometry/hittable.h"
#include "math/vec3.h"

namespace shape::procedural
{
	class sphere : public hittable
	{
	public:
		sphere() = default;
		sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

		virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

		virtual bool bounding_box(double time0, double time1, aabb &output_box) const;

		// Update u v
		static void get_sphere_uv(const point3 &p, double &u, double &v)
		{
			double theta = acos(-p.y());
			double phi = atan2(-p.z(), p.x()) + PI;

			u = phi * INV_2PI;
			v = theta * INV_PI;
		}

		virtual double pdf_value(const point3 &origin, const vec3 &v) const override;
		virtual vec3 random(const point3 &origin) const override;

	public:
		point3 center;
		double radius;
		shared_ptr<material> mat_ptr;
	};

}