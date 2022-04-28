#include "shape/procedural/cylinder.h"

#include "geometry/aabb.h"
#include "rtweekend.h"

namespace shape::procedural
{
	cylinder::cylinder(double radius, double phi_max, double zmin, double zmax, shared_ptr<material> m)
		: radius(radius), phi_max(degrees_to_radians(clamp(phi_max, 0.0, 360.0))), zmin(zmin), zmax(zmax), mat_ptr(m)
	{

	}

	// Reference from https://www.pbr-book.org/3ed-2018/Shapes/Cylinders
	bool cylinder::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		double a, b, c;
		double dx, dy, ox, oy;
		dx = r_in.direction().x();
		dy = r_in.direction().y();
		ox = r_in.origin().x();
		oy = r_in.origin().y();

		a = dx * dx + dy * dy;
		b = 2 * (dx * ox + dy * oy);
		c = ox * ox + oy * oy - radius * radius;

		double t0, t1;
		if (!quadratic(a, b, c, t0, t1))
			return false;

		if (t0 > t_max || t1 <= 0)
			return false;

		// Compute cylinder hit point and phi
		double tShapeHit = t0;
		if (tShapeHit <= 0)
		{
			tShapeHit = t1;
			if (tShapeHit > t_max)
				return false;
		}
		vec3 temp_point = r_in.at(tShapeHit);
		// Refine cylinder intersection point
		double hit_rad = std::sqrt(temp_point.x() * temp_point.x() + temp_point.y() * temp_point.y());
		double hit_rad_inv = 1.0 / hit_rad;
		temp_point.e[0] *= radius * hit_rad_inv;
		temp_point.e[1] *= radius * hit_rad_inv;

		double phi = std::atan2(temp_point.y(), temp_point.x());
		if (phi < 0)
			phi += TWO_PI;

		// Test cylinder intersection against clipping parameters
		if (temp_point.z() < zmin || temp_point.z() > zmax || phi > phi_max)
		{
			if (std::fabs(tShapeHit - t1) <= EPSILON)
				return false;
			tShapeHit = t1;
			if (t1 > t_max)
				return false;
			temp_point = r_in.at(tShapeHit);

			double hit_rad = std::sqrt(temp_point.x() * temp_point.x() + temp_point.y() * temp_point.y());
			double hit_rad_inv = 1.0 / hit_rad;
			temp_point.e[0] *= radius * hit_rad_inv;
			temp_point.e[1] *= radius * hit_rad_inv;

			phi = std::atan2(temp_point.y(), temp_point.x());
			if (phi < 0)	phi += TWO_PI;
			if (temp_point.z() < zmin || temp_point.z() > zmax || phi > phi_max)
				return false;
		}

		// Find parametric representation of cylinder hit
		double u = phi / phi_max;
		double v = (temp_point.z() - zmin) / (zmax - zmin);

		rec.t = tShapeHit;
		rec.p = temp_point;
		rec.u = u;
		rec.v = v;
		rec.mat_ptr = mat_ptr;

		// Compute cylinder ∂n/∂u and ∂n/∂v
		vec3 dpdu(-phi_max * temp_point.y(), phi_max * temp_point.x(), 0);
		vec3 dpdv(0, 0, zmax - zmin);
		rec.normal = normalize(cross(dpdu, dpdv));

		return true;
	}

	bool cylinder::bounding_box(double time0, double time1, aabb &output_box) const
	{
		point3 pmin(-radius, radius, zmin);
		point3 pmax(radius, radius, zmax);

		output_box = aabb(pmin, pmax);
		return true;
	}
}