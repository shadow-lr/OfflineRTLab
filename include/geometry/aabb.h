#pragma once

#include "../rtweekend.h"
#include "geometry/ray.h"

class aabb
{
public:
	aabb()
	{
		double minNum = std::numeric_limits<double>::lowest();
		double maxNum = std::numeric_limits<double>::max();
		maximum = point3(minNum, minNum, minNum);
		minimum = point3(maxNum, maxNum, maxNum);
	}
	aabb(const point3 &a) : minimum(a), maximum(a) {}
	aabb(const point3 &a, const point3 &b) : minimum(Min(a, b)), maximum(Max(a, b)) {}

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	bool hit(const ray &r, double t_min, double t_max) const;

	vec3 diagonal() const { return max() - min(); }
	vec3 offset(const vec3 &p) const;
	int max_extent() const;
	double get_surface_area() const
	{
		vec3 d = diagonal();
		return 2 * (d.x() * d.y() + d.x() * d.z() + d.y() * d.z());
	}
	vec3 get_centroid() { return 0.5 * min() + 0.5 * max(); }

public:
	point3 minimum, maximum;
};

// merge two aabb
aabb surrounding_box(const aabb &box0, const aabb &box1);

void printf_info(aabb& box);
