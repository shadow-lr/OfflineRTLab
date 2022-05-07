#include "geometry/bvh.h"

#include <assert.h>

bool bvh_node::hit_withnode(const ray &r, double t_min, double t_max, hit_record &rec, bvh_build_node *node) const
{
	if (node == nullptr || !node->box.hit(r, t_min, t_max))
		return false;

	bool hit_left, hit_right;
	hit_left = hit_right = false;

	if (node->left == nullptr && node->right == nullptr)
	{
		return node->obj->hit(r, t_min, t_max, rec);
	}

	hit_left = hit_withnode(r, t_min, t_max, rec, node->left);
	hit_right = hit_withnode(r, t_min, hit_left ? rec.t : t_max, rec, node->right);

	return hit_left || hit_right;
}


bool bvh_node::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
	if (!root)
		return false;
	return hit_withnode(r, t_min, t_max, rec, root);
}

bool bvh_node::bounding_box(double time0, double time1, aabb &output_box) const
{
	output_box = root->box;
	return true;
}

bvh_node::bvh_node(std::vector<shared_ptr<object>> &src_objects, double time0, double time1, SplitMethod split_method_)
	: bvh_node(src_objects, 0, src_objects.size() - 1, time0, time1, split_method_)
{
	root = recursive_build(primitives);
}

bvh_node::bvh_node(std::vector<shared_ptr<object>> &src_objects,
				   size_t start, size_t end,
				   double time0, double time1,
				   bvh_node::SplitMethod split_method_) : split_method(split_method_), primitives(std::move(src_objects))
{

}

bvh_build_node* bvh_node::recursive_build(std::vector<shared_ptr<object>> objects)
{
	bvh_build_node* node = new bvh_build_node();

	std::vector<shared_ptr<object>> leftshapes, rightshapes;

	int size = objects.size();
	if (size == 1)
	{
		objects[0]->bounding_box(0, 0,node->box);
		node->obj = objects[0];
		node->left = nullptr;
		node->right = nullptr;

		return node;
	}
	else if (size == 2)
	{
		node->left = recursive_build(std::vector{objects[0]});
		node->right = recursive_build(std::vector{objects[1]});

		node->box = surrounding_box(node->left->box, node->right->box);
		return node;
	}
	else
	{
		aabb centroid_bounds, temp;
		for (int i = 0 ; i < objects.size() ; ++ i)
		{
			// todo: add time0 and time1
			objects[i]->bounding_box(0, 0, temp);
			centroid_bounds = surrounding_box(centroid_bounds, aabb(temp.get_centroid()));
		}

		switch (split_method)
		{
			case SplitMethod::NAIVE:
			{
				build_bvh(node, objects, centroid_bounds, leftshapes, rightshapes);
				break;
			}
			case SplitMethod::SAH:
			{
				build_sah(node, objects, centroid_bounds, leftshapes, rightshapes);
				break;
			}
			default:
				break;
		}
	}

	assert(objects.size() == (leftshapes.size() + rightshapes.size()));

	node->left = recursive_build(leftshapes);
	node->right = recursive_build(rightshapes);
	node->box = surrounding_box(node->left->box, node->right->box);

	return node;
}

void bvh_node::build_bvh(bvh_build_node *&node,
						 std::vector<shared_ptr<object>> &objects,
						 aabb &centroid_bounds,
						 std::vector<shared_ptr<object>> &leftshapes,
						 std::vector<shared_ptr<object>> &rightshapes)
{
	int axis = centroid_bounds.max_extent();
	if (axis == 0)
		std::sort(objects.begin(), objects.end(), box_x_compare);
	else if (axis == 1)
		std::sort(objects.begin(), objects.end(), box_y_compare);
	else if (axis == 2)
		std::sort(objects.begin(), objects.end(), box_z_compare);

	int size = objects.size() / 2;
	auto beginning = objects.begin();
	auto middling = objects.begin() + size;
	auto ending = objects.end();

	leftshapes = std::vector<shared_ptr<object>>(beginning, middling);
	rightshapes = std::vector<shared_ptr<object>>(middling, ending);
}

void bvh_node::build_sah(bvh_build_node *&node,
						 std::vector<shared_ptr<object>> &objects,
						 aabb &centroid_bounds,
						 std::vector<shared_ptr<object>> &leftshapes,
						 std::vector<shared_ptr<object>> &rightshapes)
{
	aabb temp, bounds;
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->bounding_box(0, 0, temp);
		bounds = surrounding_box(bounds, temp);
	}

	constexpr int nBuckets = 12;
	double minCost = std::numeric_limits<double>::max();
	int minCostSplitBucket = 0;
	int minDim = 0;

	for (int dim = 0; dim < 3; ++dim)
	{
		BucketInfo buckets[nBuckets];
		for (int i = 0; i < objects.size(); ++i)
		{
			aabb temp;
			objects[i]->bounding_box(0, 0, temp);
			vec3 offsetVec = centroid_bounds.offset(temp.get_centroid());
			int b = nBuckets * offsetVec[dim];
			if (b >= nBuckets) b = nBuckets - 1;
			buckets[b].count++;
			buckets[b].bounds = surrounding_box(buckets[b].bounds, temp.get_centroid());
		}

		double cost[nBuckets - 1];
		for (int i = 0; i < nBuckets - 1; ++i)
		{
			aabb b0, b1;
			int count0 = 0, count1 = 0;
			for (int j = 0; j <= i; ++j)
			{
				if (buckets[j].count != 0)
				{
					b0 = surrounding_box(b0, buckets[j].bounds);
					count0 += buckets[j].count;
				}
			}
			for (int j = i + 1; j < nBuckets; ++j)
			{
				if (buckets[j].count != 0)
				{
					b1 = surrounding_box(b1, buckets[j].bounds);
					count1 += buckets[j].count;
				}
			}
			cost[i] = .125f + (count0 * b0.get_surface_area() + count1 * b1.get_surface_area()) / bounds.get_surface_area();

			if (cost[i] < minCost)
			{
				minCost = cost[i];
				minCostSplitBucket = i;
				minDim = dim;
			}
		}
	}

	for (shared_ptr<object>& object : objects)
	{
		object->bounding_box(0, 0, temp);
		int b = nBuckets * centroid_bounds.offset(temp.get_centroid())[minDim];
		if (b >= nBuckets) b = nBuckets - 1;
		if (b <= minCostSplitBucket)
		{
			leftshapes.push_back(object);
		}
		else
		{
			rightshapes.push_back(object);
		}
	}
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