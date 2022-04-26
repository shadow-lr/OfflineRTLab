#pragma once

#include "geometry/object.h"
#include "math/vertex.h"
#include "math/vec3.h"

#include <array>

namespace shape::model
{
//	class triangle : public hittable
//	{
//	public:
//		triangle(vec3 v0_, vec3 v1_, vec3 v2_, shared_ptr<material> ptr = nullptr)
//			: v0(v0_), v1(v1_), v2(v2_), mat_ptr(ptr)
//		{
//			e1 = v1 - v0;
//			e2 = v2 - v0;
//			normal = normalize(cross(e1, e2));
//			area = cross(e1, e2).length() * 0.5;
//		}
//
//		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
//		bool bounding_box(double time0, double time1, aabb &output_box) const override;
//
//		~triangle();
//
//		double get_area() const { return area; }
//
//	public:
//		std::array<vec3, 3> vertex;
////		vec3 v0, v1, v2; // vertices A, B ,C , counter-clockwise order
//		vec3 e1, e2;     // 2 edges v1-v0, v2-v0;
//		vec3 t0, t1, t2; // texture coords
//		vec3 normal;
//		double area;
//
//		shared_ptr<material> mat_ptr;
//	};

	class triangle : public object
	{
	public:
		triangle(vec3 v0_, vec3 v1_, vec3 v2_, shared_ptr<material> ptr = nullptr) : mat_ptr(ptr)
		{
			vertices[0].pos = v0_;
			vertices[1].pos = v1_;
			vertices[2].pos = v2_;
			vec3 normal = normalize(cross(v2_ - v0_, v1_ - v0_));
			vertices[0].normal = normal;
			vertices[1].normal = normal;
			vertices[2].normal = normal;
//			area = cross(e1, e2).length() * 0.5;
		}

		triangle(vertex& v0, vertex& v1, vertex& v2, shared_ptr<material> ptr = nullptr) : mat_ptr(ptr)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
		~triangle() override;

	public:
		std::array<vertex, 3> vertices;
		shared_ptr<material> mat_ptr;
	};
}