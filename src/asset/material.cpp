#include <geometry/obn.h>
#include "asset/material.h"

// 漫反射
bool lambertian::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
	srec.is_specular = false;
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
	return true;
}

// 出射向量的pdf
double lambertian::scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const
{
	auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
	return cosine < 0 ? 0 : cosine * INV_PI;
}

// 金属
bool metal::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
//	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
////        scattered = ray(rec.p, reflected);
//// 扰动 fuzz * random_unit_sphere
//	srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
//	srec.attenuation = albedo;
//	return (dot(srec.specular_ray.direction(), rec.normal) > 0);

	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
	srec.specular_ray = ray(rec.p, reflected+fuzz*random_in_unit_sphere(), r_in.time());
	srec.attenuation = albedo;
	srec.is_specular = true;
	srec.pdf_ptr = nullptr;
	return true;
}

// 电介质
bool dielectric::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
	srec.is_specular = true;
	srec.pdf_ptr = nullptr;
	srec.attenuation= color(1.0, 1.0, 1.0);
	double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

	// 先根据法线和入射向量计算出角度的三角函数值 然后再根据公式 n1 / n2 * sin(θ) > 1 大于1就全反射，反之 散射
	vec3 unit_direction = unit_vector(r_in.direction());
//	vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

	// new content
	// dot(v, n) = cos(θ)
	double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	vec3 direction;

	// 真正的玻璃具有与角度不同的反射率————从陡峭的角度看窗户，它就会变成一面镜子
	if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double())
	{
		// Must Reflect 必定反射 直接执行反射公式
		direction = reflect(unit_direction, rec.normal);
	}
	else
	{
		// Can refract 可以散射 走折射公式
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}

	srec.specular_ray = ray(rec.p, direction, r_in.time());
	return true;
}