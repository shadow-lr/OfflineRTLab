#pragma once

#include "geometry/pdf.h"
#include "geometry/object.h"

#include "texture.h"
#include "microfacedist.h"

// Christophe Schlick
static double schlick_fresnel(double cosine, double ref_idx)
{
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

struct hit_record;

struct scatter_record
{
	ray specular_ray;
	bool is_specular;
	color attenuation;
	shared_ptr<pdf> pdf_ptr;
};

// Tell the rays how to interact with the surface of an object
class material
{
public:
	// light
	virtual color emitted(const ray &r_in, const hit_record &rec, double u, double v, const point3 &p) const
	{
		return color(0, 0, 0);
	}

	/**
	 * @brief introduction of function
	 * @param r_in              argus 1 enter ray
	 * @param rec               argus 2 Distance of the intersection of the rays from the origin point of ray(t)、hit point(p), hit point normal.
	 * @param attenuation       argus 3 attenuation(output) color
	 * @param scattered         argus 4 scattered ray
	 *
	 * @return Return Description
	 */
	virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const
	{
		return false;
	}

	virtual double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
	{
		return 0;
	}
	virtual ~material() {}
};

class lambertian : public material
{
public:
	lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}

	lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override;
	double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override;

public:
	shared_ptr<texture> albedo;
};

class metal : public material
{
public:
	metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override;

public:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double index_of_refraction) : ior(index_of_refraction) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override;

public:
	double ior;  // Index Of Refraction
};

class isotropic : public material
{
public:
	isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

	isotropic(shared_ptr<texture> a) : albedo(a) {}

    // picks a uniform random direction
	virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override
	{
		// scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
		// attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
    shared_ptr<texture> albedo;
};

class oren_nayar : public material
{
public:
	oren_nayar(color c, double sigma) : albedo(make_shared<solid_color>(c))
	{
		double sigma2 = sigma * sigma;
		a = 1.0 - (sigma2 / (2.0 * (sigma2 + 0.33)));
		b = 0.45 * sigma2 / (sigma2 + 0.09);
	}

	oren_nayar(shared_ptr<texture> albedo_, double sigma) : albedo(albedo_)
	{
		double sigma2 = sigma * sigma;
		a = 1.0 - (sigma2 / (2.0 * (sigma2 + 0.33)));
		b = 0.45 * sigma2 / (sigma2 + 0.09);
	}

	bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override
	{
		srec.is_specular = false;
		srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
		srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
		return true;
	}

	double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override
	{
		vec3 wi = normalize(r_in.direction());
		vec3 wo = normalize(scattered.direction());

		double cosine = dot(rec.normal, wo);
		if (cosine < 0)
			cosine = 0;

		double sinThetaI = SinTheta(wi);
		double sinThetaO = SinTheta(wo);

		double maxCos = 0;
		if (sinThetaI > 1e-4 && sinThetaO > 1e-4)
		{
			double sinPhiI = SinPhi(wi);
			double cosPhiI = CosPhi(wi);
			double sinPhiO = SinPhi(wo);
			double cosPhiO = CosPhi(wo);
			double dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
			maxCos = std::max(0.0, dCos);
		}
		double sinAlpha, tanBeta;
		if (AbsCosTheta(wi) > AbsCosTheta(wo))
		{
			sinAlpha = sinThetaO;
			tanBeta = sinThetaI / AbsCosTheta(wi);
		}
		else
		{
			sinAlpha = sinThetaI;
			tanBeta = sinThetaO / AbsCosTheta(wo);
		}

		return ((a + b * maxCos * sinAlpha * tanBeta) * INV_PI * cosine);
	}
public:
	shared_ptr<texture> albedo;
	double a, b;
};

class microfacet_reflection : public material
{
public:
	microfacet_reflection(const shared_ptr<texture> &albedo,
						  const shared_ptr<microfacet_distribution> &distribution,
						  double ior);

	microfacet_reflection(color c,
						  const shared_ptr<microfacet_distribution> &distribution,
						  double ior);

	bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override;
	double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override;

public:
	shared_ptr<texture> albedo;
	shared_ptr<microfacet_distribution> distribution;
	double ior;
};