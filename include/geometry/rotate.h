#pragma once

#include "hittable_list.h"
#include "geometry/aabb.h"

class rotate : public hittable_list
{
public:
	virtual vec3 rotate_matrix(const vec3& v, bool isInverse = false) const
	{
		return vec3(0, 0, 0);
	}
	rotate(shared_ptr<object> p, double angle);

	virtual bool hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
	{
		output_box = bbox;
		return hasbox;
	}

public:
	shared_ptr<object> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
};

class rotate_x : public rotate
{
public:
	rotate_x(const shared_ptr<object> &p, double angle) : rotate(p, angle) {}
	vec3 rotate_matrix(const vec3& v, bool isInverse = false) const override;
};

class rotate_y : public rotate
{
public:
	rotate_y(const shared_ptr<object> &p, double angle) : rotate(p, angle) {}
	vec3 rotate_matrix(const vec3& v, bool isInverse = false) const override;
};

class rotate_z : public rotate
{
public:
	rotate_z(const shared_ptr<object> &p, double angle) : rotate(p, angle) {}
	vec3 rotate_matrix(const vec3& v, bool isInverse = false) const override;
};