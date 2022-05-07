#include "shape/procedural/sphere.h"
#include "geometry/aabb.h"
#include "geometry/onb.h"

namespace shape::procedural
{
	bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
	{
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = half_b * half_b - a * c;

		// No intersection
		if (discriminant < 0)
			return false;
		double sqrtd = sqrt(discriminant);

		// Find the closest root point within an acceptable range in a straight line
		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || t_max < root)
		{
			root = (-half_b + sqrtd) / a;
			if (root < t_min || t_max < root)
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = (rec.p - center) / radius;

		// The direction of the surface normal must be opposite to the incident
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = mat_ptr.get();

		get_sphere_uv(outward_normal, rec.u, rec.v);
		return true;
	}

	bool sphere::bounding_box(double time0, double time1, aabb &output_box) const
	{
		output_box = aabb(center - vec3(radius, radius, radius),
						  center + vec3(radius, radius, radius));
		return true;
	}

	// from https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html#cleaninguppdfmanagement/samplingasphereobject
	double sphere::pdf_value(const point3 &o, const vec3 &v) const
	{
		hit_record rec;
		if (!this->hit(ray(o, v), 0.0001, infinity, rec))
			return 0;

		auto cos_theta_max = std::sqrt(1 - radius * radius / (center - o).length_squared());
		auto solid_angle = TWO_PI * (1 - cos_theta_max);

		return 1 / solid_angle;
	}

	vec3 sphere::random(const point3 &origin) const
	{
		vec3 direction = center - origin;
		auto distance_squared = direction.length_squared();
		onb uvw;
		uvw.build_from_w(direction);
		return uvw.local(random_to_sphere(radius, distance_squared));
	}
}