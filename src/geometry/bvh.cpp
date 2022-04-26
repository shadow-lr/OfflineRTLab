#include "geometry/bvh.h"

bool bvh_node::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
	if (!box.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max, rec);
	bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

	// Whether the left and right nodes have intersection results and merge
	return hit_left || hit_right;
}

bool bvh_node::bounding_box(double time0, double time1, aabb &output_box) const
{
	output_box = box;
	return true;
}

bvh_node::bvh_node(const std::vector<shared_ptr<object>> &src_objects, double time0, double time1)
{
	auto objects = src_objects;
	int axis = random_double(0, 2);
	if (axis == 0)
		std::sort(objects.begin(), objects.end(), box_x_compare);
	else if (axis == 1)
		std::sort(objects.begin(), objects.end(), box_y_compare);
	else if (axis == 2)
		std::sort(objects.begin(), objects.end(), box_z_compare);

	int size = objects.size();
	if (size == 1)
	{
		left = right = objects[0];
	}
	else if (size == 2)
	{
		left = objects[0];
		right = objects[1];
	}
	else
	{
		left = make_shared<bvh_node>((std::vector<shared_ptr<object>>(objects.begin(), objects.begin() + size / 2)), time0, time1);
		right = make_shared<bvh_node>((std::vector<shared_ptr<object>>(objects.begin() + size / 2, objects.end())), time0, time1);
	}

	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
	{
		std::cerr << "No bounding box in bvh_node constructor.\n";
	}

	box = surrounding_box(box_left, box_right);
}

bool box_x_compare(const shared_ptr<object> a, const shared_ptr<object> b)
{
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<object> a, const shared_ptr<object> b)
{
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<object> a, const shared_ptr<object> b)
{
	return box_compare(a, b, 2);
}