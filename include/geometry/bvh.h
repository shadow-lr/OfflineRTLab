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

struct bvh_build_node;

class bvh_node : public object
{
public:
	// for sah
	struct BucketInfo
	{
		int count = 0;
		aabb bounds;
	};
	// SAH(surface area heuristic)
	enum class SplitMethod { NAIVE = 0, SAH };

	bvh_node() = default;
	bvh_node(std::vector<shared_ptr<object>> &src_objects,
			 double time0, double time1,
			 SplitMethod split_method_ = SplitMethod::NAIVE);
	bvh_node(std::vector<shared_ptr<object>> &src_objects,
			 size_t start, size_t end,
			 double time0, double time1,
			 SplitMethod split_method_ = SplitMethod::NAIVE);

	bool hit_withnode(const ray &r, double t_min, double t_max, hit_record &rec, bvh_build_node* node) const;
    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
	bool bounding_box(double time0, double time1, aabb &output_box) const override;

	bvh_build_node* recursive_build(std::vector<shared_ptr<object>> objects);
	void build_bvh(bvh_build_node*& node, std::vector<shared_ptr<object>>& objects, aabb& centroid_bounds, std::vector<shared_ptr<object>>& leftshapes, std::vector<shared_ptr<object>>& rightshapes);
	void build_sah(bvh_build_node*& node, std::vector<shared_ptr<object>>& objects, aabb& centroid_bounds, std::vector<shared_ptr<object>>& leftshapes, std::vector<shared_ptr<object>>& rightshapes);

public:
	SplitMethod split_method;
	bvh_build_node* root;
	std::vector<shared_ptr<object>> primitives;
};

struct bvh_build_node
{
	aabb box;
	bvh_build_node* left = nullptr;
	bvh_build_node* right = nullptr;
	shared_ptr<object> obj;
};

inline bool box_compare(const shared_ptr<object> a, const shared_ptr<object> b, int axis)
{
	aabb box_a, box_b;

	// bvh
	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min()[axis] < box_b.min()[axis];
}

