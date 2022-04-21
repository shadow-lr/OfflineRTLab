#include "shape/procedural/cube.h"

namespace shape::procedural
{
	bool cube::bounding_box(double time0, double time1, aabb &output_box) const
	{
		return false;
	}

	bool cube::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
	{
		return false;
	}

	cube::cube(point3 cen, double length, shared_ptr<material> m) : center(cen), side_length(length), mat_ptr(m)
	{
		double half_len = 0.5 * length;
		vec3 vertices[8];
		vertices[0] = cen + half_len * vec3(1, 1, 1);
		vertices[1] = cen + half_len * vec3(1, -1, 1);
		vertices[2] = cen + half_len * vec3(-1, -1, 1);
		vertices[3] = cen + half_len * vec3(-1, 1, 1);
		vertices[4] = cen + half_len * vec3(1, 1, -1);
		vertices[5] = cen + half_len * vec3(1, -1, -1);
		vertices[6] = cen + half_len * vec3(-1, -1, -1);
		vertices[7] = cen + half_len * vec3(-1, 1, -1);
	}
}

