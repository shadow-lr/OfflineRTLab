#pragma once

#include "geometry/hittable.h"
#include "asset/material.h"
#include "math/vertex.h"

#include <string>

namespace shape::model
{
	class mesh_triangle : hittable
	{
	public:
		mesh_triangle(const std::string filename, shared_ptr<material> ptr);

		bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
		double pdf_value(const point3 &o, const vec3 &v) const override;
		vec3 random(const vec3 &o) const override;
		~mesh_triangle() override;
	public:
		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;

	};
}