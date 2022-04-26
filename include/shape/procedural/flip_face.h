#pragma once

#include "geometry/object.h"

namespace shape::procedural
{
	// make normals point in the −y direction
	class flip_face : public object
	{
	public:
		flip_face(shared_ptr<object> p) : ptr(p) {}
	private:
		virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
		{
			if (!ptr->hit(r, t_min, t_max, rec))
				return false;

			rec.front_face = !rec.front_face;
			return true;
		}
		bool bounding_box(double time0, double time1, aabb &output_box) const override
		{
			return ptr->bounding_box(time0, time1, output_box);
		}
	public:
		shared_ptr<object> ptr;
	};
}