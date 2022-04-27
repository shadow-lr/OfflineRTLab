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
	bool triangle::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		double t, u, v;

		// E1
		vec3 E1 = vertices[1].pos - vertices[0].pos;
		// E2
		vec3 E2 = vertices[2].pos - vertices[0].pos;
		// P
		vec3 P = cross(r_in.direction(), E2);
		// determinant
		double det = dot(E1, P);

		// keep det > 0, modify T accordingly
		vec3 T;
		if (det > 0)
		{
			T = vec3(r_in.origin() - vertices[0].pos);
		}
		else
		{
			T = vertices[0].pos - r_in.origin();
			det = -det;
		}

		if (det < 0.0001f)
			return false;

		// u <= 1
		u = dot(T, P);
		if (u < 0.0f || u > det)
			return false;

		// Q
		vec3 Q = cross(T, E1);

		// u + v <= 1
		v = dot(r_in.direction(), Q);
		if (v < 0.0f || u + v > det)
			return false;

		t = dot(E2, Q);

		float fInvDet = 1.0f / det;
		rec.t = t * fInvDet;
		if (rec.t <= t_min || rec.t >= t_max)
			return false;
		rec.p = r_in.at(rec.t);

		u = u * fInvDet;
		v = v * fInvDet;

		rec.u = vertices[0].tex_coord.x() * (1 - u - v) + vertices[1].tex_coord.x() * u + vertices[2].tex_coord.x() * v;
		rec.v = vertices[0].tex_coord.y() * (1 - u - v) + vertices[1].tex_coord.y() * u + vertices[2].tex_coord.y() * v;

		rec.normal = vertices[0].normal * (1 - u - v) + vertices[1].normal * u + vertices[2].normal * v;

		rec.mat_ptr = mat_ptr;

		return true;
	}

	bool triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		double min_x, min_y, min_z, max_x, max_y, max_z;
		auto v0 = vertices[0].pos;
		auto v1 = vertices[1].pos;
		auto v2 = vertices[2].pos;

		min_x = min_compare(v0.x(), v1.x(), v2.x());
		min_y = min_compare(v0.y(), v1.y(), v2.y());
		min_z = min_compare(v0.z(), v1.z(), v2.z());

		max_x = max_compare(v0.x(), v1.x(), v2.x()) + 0.1;
		max_y = max_compare(v0.y(), v1.y(), v2.y()) + 0.1;
		max_z = max_compare(v0.z(), v1.z(), v2.z()) + 0.1;

		vec3 min_point(min_x, min_y, min_z);
		vec3 max_point(max_x, max_y, max_z);

		output_box = aabb(min_point, max_point);
		return true;
	}

	triangle::~triangle() noexcept
	{
		mat_ptr.reset();
	}
}
