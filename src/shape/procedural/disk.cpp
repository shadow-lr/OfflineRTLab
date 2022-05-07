#include "shape/procedural/disk.h"

#include "geometry/aabb.h"
#include "rtweekend.h"

namespace shape::procedural
{
	disk::disk(double height, double radius, double inner_radius, double phi_max, shared_ptr<material> m)
		: height(height), radius(radius), inner_radius(inner_radius), phi_max(degrees_to_radians(clamp(phi_max, 0, 360))), mat_ptr(m)
	{

	}

	// Reference from https://www.pbr-book.org/3ed-2018/Shapes/Disks
	bool disk::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		// Reject disk intersections for rays parallel to the disk's plane
		if (std::abs(r_in.direction().z()) <= EPSILON)
			return false;

		double tShapeHit = (height - r_in.origin().z()) / r_in.direction().z();
		if (tShapeHit <= 0 || tShapeHit >= t_max)
			return false;

		// See if hit point is inside disk radii and phi_max
		point3 pHit = r_in.at(tShapeHit);
		double dist2 = pHit.x() * pHit.x() + pHit.y() * pHit.y();
		if (dist2 > radius * radius || dist2 < inner_radius * inner_radius)
			return false;

		// Test disk phi value against phi_max
		double phi = std::atan2(pHit.y(), pHit.x());
		if (phi < 0) phi += TWO_PI;
		if (phi > phi_max) return false;

		// Find parametric representation of disk hit
		double u = phi / phi_max;
		double rHit = std::sqrt(dist2);
		double v = (radius - rHit) / (radius - inner_radius);
		vec3 dpdu(-phi_max * pHit.y(), phi_max * pHit.x(), 0);
		vec3 dpdv = vec3(pHit.x(), pHit.y(), 0) * (inner_radius - radius) / rHit;
		pHit[2] = height;

		rec.t = tShapeHit;
		rec.p = r_in.at(tShapeHit);
		rec.u = u;
		rec.v = v;
		rec.normal = normalize(cross(dpdu, dpdv));
		rec.mat_ptr = mat_ptr.get();

		return true;
	}

	bool disk::bounding_box(double time0, double time1, aabb &output_box) const
	{
		point3 min_point(-radius, -radius, height);
		point3 max_point(radius, radius, height);

		output_box = aabb(min_point, max_point);
		return true;
	}
}