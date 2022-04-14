#include "shape/sphere.h"

// sphere与光线求交判定
bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    // 求交
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;

    // 没根的情况下返回
    if (discriminant < 0)
        return false;
    double sqrtd = sqrt(discriminant);

    // 找到最近的root点在直线中可接受范围内
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // 更新hit_record的rec信息
    rec.t = root;
    rec.p = r.at(rec.t);
    // 中心点到原上点的向量 再 除以 半径（向量的长度）
    rec.normal = (rec.p - center) / radius;

    // 表面法线方向一定与入射相反的
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    get_sphere_uv(outward_normal, rec.u, rec.v);
    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius),
					  center + vec3(radius, radius, radius));
    return true;
}

// https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html#cleaninguppdfmanagement/samplingasphereobject
double sphere::pdf_value(const point3 &o, const vec3 &v) const {
	hit_record rec;
	if (!this->hit(ray(o, v), 0.0001, infinity, rec))
		return 0;

	auto cos_theta_max = std::sqrt(1 - radius * radius / (center - o).length_squared());
	auto solid_angle = TWO_PI * (1 - cos_theta_max);

	return 1 / solid_angle;
}

vec3 sphere::random(const point3 &o) const {
	vec3 direction = center - o;
	auto distance_squared = direction.length_squared();
	onb uvw;
	uvw.build_from_w(direction);
	return uvw.local(random_to_sphere(radius, distance_squared));
}