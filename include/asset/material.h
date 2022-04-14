#pragma once

#include <geometry/pdf.h>
#include "rtweekend.h"
#include "geometry/hittable.h"
#include "texture.h"

struct hit_record;

struct scatter_record {
	ray specular_ray;
	bool is_specular;
	color attenuation;
	shared_ptr<pdf> pdf_ptr;
};

// 告诉射线如何与表面相互作用
class material {
public:
    // 对于可自发光的材质(light)
    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3 &p) const {
        return color(0, 0, 0);
    }

	/**
	 * @brief 函数简要说明-测试函数
	 * @param r_in              参数1 射线
	 * @param rec               参数2 离光线起点的距离t、撞点的坐标向量p、撞点出的法向量normal.
	 * @param attenuation       参数3 衰弱后的颜色值
	 * @param scattered         参数4 散射射线
	 *
	 * @return 返回说明
	 */
	virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const {
		return false;
	}

	virtual double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const {
		return 0;
	}
	virtual ~material() {}
};

// 兰伯特模型类
class lambertian : public material {
public:
    lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}

    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record& srec) const override;

	double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override;

public:
    shared_ptr<texture> albedo;
};

// 金属类
class metal : public material {
public:
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record& srec) const override;

public:
    color albedo;
    double fuzz;
};

// dielectric电介质类
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record& srec) const override;

public:
    // 折射率
    double ir;  // Index Of Refraction
private:
    // Christophe Schlick
    // 使用 Schlick 近似计算反射率
    static double reflectance(double cosine, double ref_idx) {
        double r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class isotropic : public material {
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

    isotropic(shared_ptr<texture> a) : albedo(a) {}

    // picks a uniform random direction
    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record& srec) const override {
//        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
//        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};