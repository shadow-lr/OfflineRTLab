#include "shape/procedural/paraboloid.h"

#include "geometry/aabb.h"
#include "rtweekend.h"

namespace shape::procedural
{
	paraboloid::paraboloid(double radius, double zmin, double zmax, double phi_max, shared_ptr<material> m)
		: radius(radius), zmin(zmin), zmax(zmax), phi_max(degrees_to_radians(clamp(phi_max, 0, 360))), mat_ptr(m)
	{

	}

	// Reference from https://www.pbr-book.org/3ed-2018/Shapes/Other_Quadrics
	bool paraboloid::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		double ox, oy, oz, dx, dy, dz;
		ox = r_in.origin().x();
		oy = r_in.origin().y();
		oz = r_in.origin().z();
		dx = r_in.direction().x();
		dy = r_in.direction().y();
		dz = r_in.direction().z();

		double k = zmax / (radius * radius);
		double a = k * (dx * dx + dy * dy);
		double b = 2 * k * (dx * ox + dy * oy) - dz;
		double c = k * (ox * ox + oy * oy) - oz;

		double t0, t1;
		if (!quadratic(a, b, c, t0, t1))
			return false;

		if (t0 > t_max || t1 <= 0)
			return false;

		double tShapeHit = t0;
		if (tShapeHit <= 0)
		{
			tShapeHit = t1;
			if (tShapeHit > t_max)
				return false;
		}

		// Compute paraboloid inverse mapping
		vec3 pHit = r_in.at(tShapeHit);
		double phi = std::atan2(pHit.y(), pHit.x());
		if (phi < 0) phi += TWO_PI;

		if (pHit.z() < zmin || pHit.z() > zmax || phi > phi_max)
		{
			if (std::abs(tShapeHit - t1) <= EPSILON)
				return false;
			tShapeHit = t1;
			if (t1 > t_max)
				return false;

			pHit = r_in.at(tShapeHit);
			phi = std::atan2(pHit.y(), pHit.x());
			if (phi < 0.0) phi += TWO_PI;
			if (pHit.z() < zmin || pHit.z() > zmax || phi > phi_max) return false;
		}

		// Find parametric representation of paraboloid hit
		double u = phi / phi_max;
		double v = (pHit.z() - zmin) / (zmax - zmin);

		vec3 dpdu(-phi_max * pHit.y(), phi_max * pHit.x(), 0.0);
		vec3 dpdv = (zmax - zmin) * vec3(pHit.x() / (2 * pHit.z()), pHit.y() / (2 * pHit.z()), 1.0);

		rec.t = tShapeHit;
		rec.p = r_in.at(tShapeHit);
		rec.u = u;
		rec.v = v;
		rec.normal = normalize(cross(dpdu, dpdv));
		rec.mat_ptr = mat_ptr;

		return false;
	}

	bool paraboloid::bounding_box(double time0, double time1, aabb &output_box) const
	{
		point3 min_point(-radius, -radius, zmin);
		point3 max_point(radius, radius, zmax);

		output_box = aabb(min_point, max_point);
		return true;
	}
}
