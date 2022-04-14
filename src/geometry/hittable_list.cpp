#include "geometry/hittable_list.h"

/* 遍历objects中所有对象，与当前的射线进行相交检测*/
bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : this->objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }


/*依次判断列表中所有物体是否被光线撞到，每次判断一个。若有被撞到，则将撞点信息保存在hit_record结构体中。我们可以看到rec是可能被写多次的，最终保存的值是后一次的值，也就是真正有效的值是后一次的值，也就是离观测点最近的物体的有效撞点（“有效撞点”：对于单个物体，会筛选出一个局部有效撞点；对于多个物体，从所有单个物体各自的局部有效撞点筛选出最终一个整体有效撞点）。因为不管这条光线依次撞击了多少个物体产生多少个撞点，我们能看到的只是离我们最近的撞点*/
/*如果当前撞点在范围内，则将当前撞点的距离设置为范围的最大值。也就是后面只考虑比该撞点更近的撞点。趋势是：找到的撞点是越来越近的，最终找到最近的撞点。*/
    return hit_anything;
}

/* 动态地BVH构造 返回的output_box为遍历hittable中所有object的包围盒之和的合并*/
bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if (this->objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    /* 如果hittable_list中objects只有一个 那就直接返回temp_box 反之不停地surrounding_box合并包围盒 */
    for (const auto &object : this->objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}

double hittable_list::pdf_value(const point3& o, const vec3& v) const {
	auto weight = 1.0/objects.size();
	auto sum = 0.0;

	for (const auto& object : objects)
		sum += weight * object->pdf_value(o, v);

	return sum;
}


vec3 hittable_list::random(const vec3 &o) const {
	auto int_size = static_cast<int>(objects.size());
	return objects[random_int(0, int_size-1)]->random(o);
}