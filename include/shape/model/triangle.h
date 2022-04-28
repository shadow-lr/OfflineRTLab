#pragma once

#include "geometry/object.h"
#include "math/vertex.h"
#include "math/vec3.h"

#include <array>

namespace shape::model
{
	class triangle : public object
	{
	public:
		triangle() {}
		triangle(vec3 v0_, vec3 v1_, vec3 v2_, shared_ptr<material> ptr = nullptr) : mat_ptr(ptr)
		{
			vertices[0].pos = v0_;
			vertices[1].pos = v1_;
			vertices[2].pos = v2_;
			vec3 normal = normalize(cross(v2_ - v0_, v1_ - v0_));
			vertices[0].normal = normal;
			vertices[1].normal = normal;
			vertices[2].normal = normal;

			e1 = vertices[1].pos - vertices[0].pos;
			e2 = vertices[2].pos - vertices[0].pos;
//			area = cross(e1, e2).length() * 0.5;
		}

		triangle(vertex& v0, vertex& v1, vertex& v2, shared_ptr<material> ptr = nullptr) : mat_ptr(ptr)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;

			e1 = vertices[1].pos - vertices[0].pos;
			e2 = vertices[2].pos - vertices[0].pos;
		}

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
		bool bounding_box(double time0, double time1, aabb &output_box) const override;
		~triangle() override;

	public:
		std::array<vertex, 3> vertices;
		vec3 e1, e2;
		shared_ptr<material> mat_ptr;
	};
}