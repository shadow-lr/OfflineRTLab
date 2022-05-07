#include "geometry/aabb.h"

bool aabb::hit(const ray &r, double t_min, double t_max) const
{
	vec3 invD = 1.0f / r.direction();

	auto t0 = (min() - r.origin()) * invD;
	auto t1 = (max() - r.origin()) * invD;

	t_min = std::max(t_min, Max(Min(t0, t1)));
	t_max = std::min(t_max, Min(Max(t0, t1)));

	return t_max > t_min;
}

vec3 aabb::offset(const vec3 &p) const
{
	vec3 o = p - min();
	o[0] /= (max().x() - min().x() + 0.001);
	o[1] /= (max().y() - min().y() + 0.001);
	o[2] /= (max().z() - min().z() + 0.001);
	return o;
}

int aabb::max_extent() const
{
	vec3 d = diagonal();
	if (d.x() > d.y() && d.x() > d.z())
		return 0;
	else if (d.y() > d.z())
		return 1;
	else
		return 2;
}

aabb surrounding_box(const aabb &box0, const aabb &box1)
{
	point3 small(fmin(box0.min().x(), box1.min().x()),
				 fmin(box0.min().y(), box1.min().y()),
				 fmin(box0.min().z(), box1.min().z()));

	point3 big(fmax(box0.max().x(), box1.max().x()),
			   fmax(box0.max().y(), box1.max().y()),
			   fmax(box0.max().z(), box1.max().z()));

	return aabb(small, big);
}

void printf_info(aabb &box)
{
	const auto& minpoint = box.min();
	const auto& maxpoint = box.max();
	printf("aabb: min %f, %f, %f  max %f, %f, %f\n", minpoint.x(), minpoint.y(), minpoint.z(), maxpoint.x(), maxpoint.y(), maxpoint.z());
}
