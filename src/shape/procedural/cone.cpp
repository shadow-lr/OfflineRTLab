#include "shape/procedural/cone.h"

#include "rtweekend.h"
#include "geometry/aabb.h"

namespace shape::procedural
{
	cone::cone(double height, double radius, double phi_max, shared_ptr<material> m)
		: height(height), radius(radius), phi_max(degrees_to_radians(clamp(phi_max, 0, 360))), mat_ptr(m)
	{

	}

	// Reference from https://www.pbr-book.org/3ed-2018/Shapes/Other_Quadrics
	bool cone::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		double ox, oy, oz, dx, dy, dz;
		ox = r_in.origin().x();
		oy = r_in.origin().y();
		oz = r_in.origin().z();
		dx = r_in.direction().x();
		dy = r_in.direction().y();
		dz = r_in.direction().z();

		double k = radius / height;
		k = k * k;

		double a = dx * dx + dy * dy - k * dz * dz;
		double b = 2 * (dx * ox + dy * oy - k * dz * (oz - height));
		double c = ox * ox + oy * oy - k * (oz - height) * (oz - height);

		double t0, t1;
		if (!quadratic(a, b, c, t0, t1))
			return false;

		if (t0 > t_max || t1 < t_min)
			return false;

		// Check quadric shape t0 and t1 for nearest inerseciton
		double tShapeHit = t0;
		if (tShapeHit < t_min)
		{
			tShapeHit = t1;
			if (tShapeHit > t_max)
				return false;
		}

		// Compute cone inverse mapping
		vec3 pHit = r_in.at(tShapeHit);
		double phi = std::atan2(pHit.y(), pHit.x());
		if (phi < .0) phi += TWO_PI;

		// Test cone intersection against clipping parameters
		if (pHit.z() < 0 || pHit.z() > height || phi > phi_max)
		{
			if (std::abs(tShapeHit - t1) <= EPSILON)
				return false;
			tShapeHit = t1;
			if (t1 > t_max) return false;
			// Compute cone inverse mapping
			pHit = r_in.at(tShapeHit);
			phi = std::atan2(pHit.y(), pHit.x());
			if (phi < .0) phi += TWO_PI;
			if (pHit.z() < 0 || pHit.z() > height || phi > phi_max)
				return false;
		}

		// Find parametric representation of cone hit
		double u = phi / phi_max;
		double v = pHit.z() / height;

		// Compute cone dpdu and dpdv
		vec3 dpdu(-phi_max * pHit.y(), phi_max * pHit.x(), 0);
		vec3 dpdv(-pHit.x() / (1.0 - v), -pHit.y() / (1.0 - v), height);

		rec.t = tShapeHit;
		rec.p = r_in.at(tShapeHit);
		rec.u = u;
		rec.v = v;
		rec.normal = normalize(cross(dpdu, dpdv));
		rec.mat_ptr = mat_ptr.get();

		return true;
	}

	bool cone::bounding_box(double time0, double time1, aabb &output_box) const
	{
		point3 min_point(-radius, -radius, 0);
		point3 max_point(radius, radius, height);

		output_box = aabb(min_point, max_point);
		return true;
	}
}

