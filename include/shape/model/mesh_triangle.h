#pragma once

#include "geometry/bvh.h"
#include "geometry/object.h"
#include "geometry/aabb.h"
#include "asset/material.h"
#include "math/vertex.h"
#include "shape/model/triangle.h"

#include <string>
#include <memory>

namespace shape::model
{
	class mesh_triangle : public object
	{
	public:
		mesh_triangle(const std::string& filename, shared_ptr<material> ptr, vec3 translate = {0.0}, vec3 scale = {1.0});

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
		~mesh_triangle() override;

		void load_obj(const std::string& path, shared_ptr<material> mt, vec3& translate, vec3& scale);
		void load_tiny_obj(const std::string& path, shared_ptr<material> mt, vec3 translate = {0.0}, vec3 scale = {1.0});
	public:
		std::vector<shared_ptr<triangle>> triangles;
		double area;

		bvh_node bvh_tree;
	};
}