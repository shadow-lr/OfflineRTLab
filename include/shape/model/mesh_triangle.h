#pragma once

#include "geometry/bvh.h"
#include "geometry/hittable.h"
#include "geometry/aabb.h"
#include "asset/material.h"
#include "math/vertex.h"
#include "shape/model/triangle.h"

#include <string>
#include <memory>

namespace shape::model
{
	class mesh_triangle : public hittable
	{
	public:
		mesh_triangle(const std::string& filename, shared_ptr<material> ptr, vec3 translate = {0.0}, vec3 scale = {1.0});

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
		~mesh_triangle() override;

		void load_obj(const std::string& path, shared_ptr<material> mt, vec3& translate, vec3& scale);
	public:
//		std::vector<vertex> vertices;
//		std::vector<uint32_t> indices;
		std::vector<shared_ptr<triangle>> triangles;
		aabb mesh_box;
		double area;

		bvh_node bvh_tree;
	};
}