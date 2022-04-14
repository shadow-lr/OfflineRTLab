#include "shape/aarect.h"

bool xy_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
	auto t = (k - r.orig.z()) / r.direction().z();

	if (t < t_min || t > t_max)
		return false;

	// hit point
	double hit_x = r.origin().x() + t * r.direction().x();
	double hit_y = r.origin().y() + t * r.direction().y();

	if (hit_x < x0 || hit_x > x1 || hit_y < y0 || hit_y > y1)
		return false;

	rec.u = (hit_x - x0) / (x1 - x0);
	rec.v = (hit_y - y0) / (y1 - y0);
	rec.t = t;
	// todo: may cause some error(z )
	auto outward_normal = vec3(0, 0, 1);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}

bool xz_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
	auto t = (k - r.origin().y()) / r.direction().y();
	if (t < t_min || t > t_max)
		return false;
	auto x = r.origin().x() + t * r.direction().x();
	auto z = r.origin().z() + t * r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	// 默认y轴向上的法线
	auto outward_normal = vec3(0, 1, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}

double xz_rect::pdf_value(const point3 &o, const vec3 &v) const {
	hit_record rec;

	if (!this->hit(ray(o, v), 0.001, infinity, rec))
		return 0;

	auto normalize_v = unit_vector(v);
	auto area = (x1 - x0) * (z1 - z0);
//	auto distance_squared = (rec.p - o).length_squared();
	auto distance_squared = rec.t * rec.t * v.length_squared();
	auto cosine = fabs(dot(normalize_v, rec.normal));

	return distance_squared / (cosine * area);
}

vec3 xz_rect::random(const vec3 &o) const {
	vec3 random_point = vec3(random_double(x0, x1), k, random_double(z0, z1));
	return random_point - o;
}

bool yz_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
	auto t = (k - r.origin().x()) / r.direction().x();
	if (t < t_min || t > t_max)
		return false;
	auto y = r.origin().y() + t * r.direction().y();
	auto z = r.origin().z() + t * r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = vec3(1, 0, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}
