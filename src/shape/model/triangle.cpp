#include "shape/model/triangle.h"
#include "geometry/aabb.h"

namespace
{
	double min_compare(double a, double b, double c)
	{
		return std::min(a, std::min(b, c));
	}

	double max_compare(double a, double b, double c)
	{
		return std::max(a, std::max(b, c));
	}
}

namespace shape::model
{
	bool shape::model::triangle::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		// todo:
		double u, v;
		vec3 pvec = cross(r_in.direction(), e2);
		double det = dot(e1, pvec);
		if (det == 0 || det < 0)
			return false;

		vec3 tvec = r_in.origin() - v0;
		u = dot(tvec, pvec);
		if (u < 0 || u > det)
			return false;

		vec3 qvec = cross(tvec, e1);
		v = dot(r_in.direction(), qvec);
		if (v < 0 || u + v > det)
			return false;

		double invDet = 1 / det;

		// closest
		double tnear = dot(e2, qvec) * invDet;
		u *= invDet;
		v *= invDet;

		if (tnear < t_min || tnear > t_max)
			return false;

		rec.t = tnear;
		rec.u = u;
		rec.v = v;
		rec.normal = normal;

		return true;
	}

	bool shape::model::triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		double min_x, min_y, min_z, max_x, max_y, max_z;
		min_x = min_compare(v0.x(), v1.x(), v2.x());
		min_y = min_compare(v0.y(), v1.y(), v2.y());
		min_z = min_compare(v0.z(), v1.z(), v2.z());

		max_x = max_compare(v0.x(), v1.x(), v2.x());
		max_y = max_compare(v0.y(), v1.y(), v2.y());
		max_z = max_compare(v0.z(), v1.z(), v2.z());

		vec3 min_point(min_x, min_y, min_z);
		vec3 max_point(max_x, max_y, max_z);

		output_box = aabb(min_point, max_point);
		return false;
	}

	shape::model::triangle::~triangle() { mat_ptr.reset(); }
}
