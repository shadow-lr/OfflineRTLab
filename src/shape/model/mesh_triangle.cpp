#include "shape/model/mesh_triangle.h"

namespace shape::model
{
	bool mesh_triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
	{
		return false;
	}

	bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		return false;
	}

	double mesh_triangle::pdf_value(const point3 &o, const vec3 &v) const
	{
		return hittable::pdf_value(o, v);
	}

	vec3 mesh_triangle::random(const vec3 &o) const
	{
		return hittable::random(o);
	}

	mesh_triangle::~mesh_triangle()
	{
	}
}

