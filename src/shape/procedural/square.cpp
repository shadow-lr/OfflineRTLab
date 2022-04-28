#include "geometry/aabb.h"
#include "shape/procedural/square.h"

namespace shape::procedural
{
	bool square::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = t_max;

		for (int i = 0; i < 2; ++i)
		{
			if (triangles[i].hit(r_in, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;

				vec3 pp = rec.p - vertices[3];
				rec.u = dot(pp, e1) / (e1.length_squared());
				rec.v = dot(pp, e2) / (e2.length_squared());
			}
		}
		return hit_anything;
	}

	bool square::bounding_box(double time0, double time1, aabb &output_box) const
	{
		aabb temp1, temp2;
		triangles[0].bounding_box(0, 0, temp1);
		triangles[1].bounding_box(0, 0, temp2);
		output_box = surrounding_box(temp1, temp2);
		return true;
	}
}