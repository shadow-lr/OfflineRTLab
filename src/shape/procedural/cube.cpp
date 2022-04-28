#include "geometry/aabb.h"
#include "shape/procedural/cube.h"

namespace shape::procedural
{
	// v7 ———————— v3
	// | \          |\
	// |  \         | \
	// |   \		|  \
	// |	\       |   \
	// |	 v4 ————|—— v0
	// |	 |		|    |
	// v6 ———|————— v2   |
	//   \	 |	     \   |
	//	  \	 |	      \  |
	//	   \ |	       \ |
	//		\|          \|
	//		 v5 ———————— v1
	/*
	triangle:
	front:
	 	v3 -> v0 -> v1
	 	v3 -> v1 -> v2
	up:
	 	v7 -> v4 -> v0
	 	v7 -> v0 -> v3
	left:
	 	v7 -> v3 -> v2
	 	v7 -> v2 -> v6
	right:
		v0 -> v4 -> v5
	 	v0 -> v5 -> v1
	bottom:
		v2 -> v1 -> v5
	 	v2 -> v5 -> v6
	back:
	 	v4 -> v7 -> v6
	 	v4 -> v6 -> v5
	 * */
	cube::cube(point3 cen, double length, shared_ptr<material> m) : center(cen), side_length(length), mat_ptr(m)
	{
		double half_len = 0.5 * side_length;
		vec3 vertices[8];
		vertices[0] = center + half_len * vec3(1, 1, 1);
		vertices[1] = center + half_len * vec3(1, -1, 1);
		vertices[2] = center + half_len * vec3(-1, -1, 1);
		vertices[3] = center + half_len * vec3(-1, 1, 1);
		vertices[4] = center + half_len * vec3(1, 1, -1);
		vertices[5] = center + half_len * vec3(1, -1, -1);
		vertices[6] = center + half_len * vec3(-1, -1, -1);
		vertices[7] = center + half_len * vec3(-1, 1, -1);

		// front
		squares.emplace_back(square(vertices[3], vertices[0], vertices[1], vertices[2], mat_ptr));
		// up
		squares.emplace_back(square(vertices[7], vertices[4], vertices[0], vertices[3], mat_ptr));
		// left
		squares.emplace_back(square(vertices[7], vertices[3], vertices[2], vertices[6], mat_ptr));
		// right
		squares.emplace_back(square(vertices[0], vertices[4], vertices[5], vertices[1], mat_ptr));
		// bottom
		squares.emplace_back(square(vertices[2], vertices[1], vertices[5], vertices[6], mat_ptr));
		// back
		squares.emplace_back(square(vertices[4], vertices[7], vertices[6], vertices[5], mat_ptr));
	}

	bool cube::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = t_max;

		for (auto &square : squares)
		{
			if (square.hit(r_in, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	bool cube::bounding_box(double time0, double time1, aabb &output_box) const
	{
		aabb temp_box;

		for (auto &square : squares)
		{
			if (square.bounding_box(time0, time1, temp_box))
			{
				output_box = surrounding_box(output_box, temp_box);
			}
			else
				return false;
		}

		return true;
	}
}

