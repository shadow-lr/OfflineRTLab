#pragma once

#include "rtweekend.h"
#include "geometry/hittable.h"
#include "geometry/hittable_list.h"

#include <algorithm>

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

// 3.8 BVH节点类
class bvh_node : public hittable {
public:
    bvh_node() = default;

    bvh_node(const hittable_list &list, double time0, double time1) : bvh_node(list.objects, 0, list.objects.size(),
                                                                               time0, time1) {}

    bvh_node(const std::vector<shared_ptr<hittable>> &src_objects, size_t start, size_t end, double time0, double time1);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    // 左节点
    shared_ptr<hittable> left;
    // 右节点
    shared_ptr<hittable> right;
    aabb box;
};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    // 动态构建BVH树
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

