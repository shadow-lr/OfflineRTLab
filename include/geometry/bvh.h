#pragma once

#include "rtweekend.h"
#include "geometry/object.h"
#include "geometry/hittable_list.h"
#include "geometry/aabb.h"

#include <algorithm>

inline bool box_compare(const shared_ptr<object> a, const shared_ptr<object> b, int axis);

bool box_x_compare(const shared_ptr<object> a, const shared_ptr<object> b);
bool box_y_compare(const shared_ptr<object> a, const shared_ptr<object> b);
bool box_z_compare(const shared_ptr<object> a, const shared_ptr<object> b);

class bvh_node : public object
{
public:
	bvh_node() = default;
    bvh_node(const std::vector<shared_ptr<object>> &src_objects, double time0, double time1);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
	bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
	// left node
	shared_ptr<object> left;
	// right node
	shared_ptr<object> right;
	aabb box;
};

inline bool box_compare(const shared_ptr<object> a, const shared_ptr<object> b, int axis)
{
	aabb box_a, box_b;

	// bvh
	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min().e[axis] < box_b.min().e[axis];
}

