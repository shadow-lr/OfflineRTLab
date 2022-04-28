#pragma once

#include "shape/model/triangle.h"
#include "math/vertex.h"

namespace shape::procedural
{
	// square's vertex layout
	/*  v1 ----	v2
	 *  |	  /	|
	 * 	|	/	|
	 * 	| /		|
		v0 ----	v3
	 triangle:
	 	v0 v1 v2
	 	v0 v2 v3
	*/
	class square : public object
	{
	public:
		square() {}
		square(vec3 v0, vec3 v1, vec3 v2, vec3 v3, shared_ptr<material> m) : mat_ptr(m)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			vertices[3] = v3;
			triangles[0] = model::triangle(v0, v1, v2, mat_ptr);
			triangles[1] = model::triangle(v0, v2, v3, mat_ptr);

			// two edge
			e1 = vertices[2] - vertices[3];
			e2 = vertices[0] - vertices[3];
		}

		bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;

		bool bounding_box(double time0, double time1, aabb &output_box) const override;

	private:
		vec3 vertices[4];
		model::triangle triangles[2];
		shared_ptr<material> mat_ptr;

		vec3 e1, e2;
	};
}