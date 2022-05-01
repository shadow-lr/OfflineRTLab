#include "geometry/rotate.h"

rotate::rotate(shared_ptr<object> p, double angle) : ptr(p)
{
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);

	point3 min(infinity, infinity, infinity);
	point3 max(-infinity, -infinity, -infinity);

	// traversal eight point
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

				// only rotate one axis
				vec3 tester = rotate_matrix(vec3(x, y, z));

				for (int c = 0; c < 3; c++)
				{
					min[c] = std::fmin(min[c], tester[c]);
					max[c] = std::fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
{
	auto origin = r_in.origin();
	auto direction = r_in.direction();

	origin = rotate_matrix(r_in.origin());
	direction = rotate_matrix(r_in.direction());

	ray rotated_r(origin, direction, r_in.time());

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p = rotate_matrix(p, true);
	normal = rotate_matrix(normal, true);

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);

	return true;
}

vec3 rotate_x::rotate_matrix(const vec3 &v, bool isInverse) const
{
	double b, c;
	if (isInverse)
	{
		b = cos_theta * v[1] + sin_theta * v[2];
		c = -sin_theta * v[1] + cos_theta * v[2];
	}
	else
	{
		b = cos_theta * v[1] - sin_theta * v[2];
		c = sin_theta * v[1] + cos_theta * v[2];
	}

	return vec3(v[0], b, c);
}

vec3 rotate_y::rotate_matrix(const vec3 &v, bool isInverse) const
{
	double a, c;
	if (isInverse)
	{
		a = cos_theta * v[0] - sin_theta * v[2];
		c = sin_theta * v[0] + cos_theta * v[2];
	}
	else
	{
		a = cos_theta * v[0] + sin_theta * v[2];
		c = -sin_theta * v[0] + cos_theta * v[2];
	}

	return vec3(a, v[1], c);
}

vec3 rotate_z::rotate_matrix(const vec3 &v, bool isInverse) const
{
	double a, b;
	if (isInverse)
	{
		a = cos_theta * v[0] - sin_theta * v[1];
		b = sin_theta * v[0] + cos_theta * v[1];
	}
	else
	{
		a = cos_theta * v[0] + sin_theta * v[1];
		b = -sin_theta * v[0] + cos_theta * v[1];
	}
	return vec3(a, b, v[2]);
}