#pragma once

#include "ray.h"
#include "aabb.h"

#include "../rtweekend.h"

class material;

/*该结构体记录“撞点”处的信息：离光线起点的距离t、撞点的坐标向量p、撞点出的法向量normal.*/
struct hit_record {
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	// 物体命中点的U,V表面坐标
	double u, v;
	bool front_face;

	// 如果射线和法线的方向相同，则该射线在对象内部，如果射线和法线的方向相反，则该射线在对象之外

	// 该决定取决于您是要在几何相交时还是在着色时确定表面的侧面，如果反射与入射反向front_face = true
	// 反射：font_face true 折射：front_face false
	// normal：朝外
	// r.direction和朝外normal在半球以外的话，则front_face = false
	inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

//hitable这个类表示能够被光线撞上的任何物体。比如，球体
class hittable {
public:
	// 是否击中
	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

	// 相交判断
	virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0;

	// pdf
	virtual double pdf_value(const point3 &o, const vec3 &v) const {
		return 0.0;
	}

	virtual vec3 random(const vec3 &o) const {
		return vec3(1, 0, 0);
	}

	virtual ~hittable() {}
};

// make normals point in the −y direction
class flip_face : public hittable {
public:
	flip_face(shared_ptr<hittable> p) : ptr(p) {}
private:

	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override {
		if (!ptr->hit(r, t_min, t_max, rec))
			return false;

		rec.front_face = !rec.front_face;
		return true;
	}
	bool bounding_box(double time0, double time1, aabb &output_box) const override {
		return ptr->bounding_box(time0, time1, output_box);
	}
public:
	shared_ptr<hittable> ptr;
};
