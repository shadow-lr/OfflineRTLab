#include "shape/procedural/hyperboloid.h"

#include "geometry/aabb.h"
#include "rtweekend.h"

namespace shape::procedural
{
	hyperboloid::hyperboloid(point3 point1, point3 point2, double phi, shared_ptr<material> m)
		: p1(point1), p2(point2), mat_ptr(m)
	{
		p1 = point1;
		p2 = point2;
		phi_max = degrees_to_radians(clamp(phi, 0, 360));
		double radius1 = std::sqrt(p1.x() * p1.x() + p1.y() * p1.y());
		double radius2 = std::sqrt(p2.x() * p2.x() + p2.y() * p2.y());
		r_max = std::max(radius1, radius2);
		zmin = std::min(p1.z(), p2.z());
		zmax = std::max(p1.z(), p2.z());
		// Compute implicit function coefficients for hyperboloid
		if (p2.z() <= 0)
			std::swap(p1, p2);

		point3 pp = p1;
		double xy1, xy2;
		do
		{
			pp += 2.0 * (p2 - p1);
			xy1 = pp.x() * pp.x() + pp.y() * pp.y();
			xy2 = p2.x() * p2.x() + p2.y() * p2.y();
			ah = (1.f / xy1 - (pp.z() * pp.z()) / (xy1 * p2.z() * p2.z())) /
				(1 - (xy2 * pp.z() * pp.z()) / (xy1 * p2.z() * p2.z()));
			ch = (ah * xy2 - 1) / (p2.z() * p2.z());
		} while (std::isinf(ah) || std::isnan(ah));
	}

	// Reference from https://www.pbr-book.org/3ed-2018/Shapes/Other_Quadrics
	bool hyperboloid::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		double ox, oy, oz, dx, dy, dz;
		ox = r_in.origin().x();
		oy = r_in.origin().y();
		oz = r_in.origin().z();
		dx = r_in.direction().x();
		dy = r_in.direction().y();
		dz = r_in.direction().z();

		double a = ah * dx * dx + ah * dy * dy - ch * dz * dz;
		double b = 2.f * (ah * dx * ox + ah * dy * oy - ch * dz * oz);
		double c = ah * ox * ox + ah * oy * oy - ch * oz * oz - 1.f;

		double t0, t1;
		if (!quadratic(a, b, c, t0, t1))
			return false;

		if (t0 > t_max || t1 < t_min)
			return false;

		double tShapeHit = t0;
		if (tShapeHit < t_min)
		{
			tShapeHit = t1;
			if (tShapeHit > t_max)
				return false;
		}

		// Compute hyperboloid inverse mapping
		vec3 pHit = r_in.at(tShapeHit);
		double v = (pHit.z() - p1.z());
		vec3 pr = (1 - v) * p1 + v * p2;
		double phi = std::atan2(pr.x() * pHit.y() - pHit.x() * pr.y(),
						  pHit.x() * pr.x() + pHit.y() * pr.y());
		if (phi < 0) phi += TWO_PI;

		// Test hyperboloid intersection against clipping parameters
		if (pHit.z() < zmin || pHit.z() > zmax || phi > phi_max)
		{
			if (std::abs(tShapeHit - t1 ) <= EPSILON)
				return false;
			tShapeHit = t1;
			if (t1 > t_max)
				return false;

			pHit = r_in.at(tShapeHit);
			v = (pHit.z() - p1.z()) / (p2.z() - p1.z());
			vec3 pr = (1 - v) * p1 + v * p2;
			phi = std::atan2(pr.x() * pHit.y() - pHit.x() * pr.y(),
							 pHit.x() * pr.x() + pHit.y() * pr.y());
			if (phi < 0.0) phi += TWO_PI;
			if (pHit.z() < zmin || pHit.z() > zmax || phi > phi_max) return false;
		}

		// Find parametric representation of paraboloid hit
		double u = phi / phi_max;

		double cos_phi = std::cos(phi), sin_phi = std::sin(phi);
		vec3 dpdu(-phi_max * pHit.y(), phi_max * pHit.x(), 0.0);
		vec3 dpdv((p2.x() - p1.x()) * cos_phi - (p2.y() - p1.y()) * sin_phi,
				  (p2.x() - p1.x()) * sin_phi + (p2.y() - p1.y()) * cos_phi, p2.z() - p1.z());

		rec.t = tShapeHit;
		rec.p = r_in.at(tShapeHit);
		rec.u = u;
		rec.v = v;
		rec.normal = normalize(cross(dpdu, dpdv));
		rec.mat_ptr = mat_ptr;

		return true;
	}

	bool hyperboloid::bounding_box(double time0, double time1, aabb &output_box) const
	{
		point3 min_point(-r_max, -r_max, zmin);
		point3 max_point(r_max, r_max, zmax);

		output_box = aabb(min_point, max_point);
		return true;
	}
}