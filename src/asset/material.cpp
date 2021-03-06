#include "asset/material.h"

// Diffuse reflection
bool lambertian::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
	srec.is_specular = false;
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
	return true;
}

// scattering pdf
double lambertian::scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const
{
	auto cosine = dot(rec.normal, normalize(scattered.direction()));
	return cosine < 0 ? 0 : cosine * INV_PI;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
	vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
	srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
	srec.attenuation = albedo;
	srec.is_specular = true;
	srec.pdf_ptr = nullptr;
	return true;
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
{
	srec.is_specular = true;
	srec.pdf_ptr = nullptr;
	srec.attenuation= color(1.0, 1.0, 1.0);
	double refraction_ratio = rec.front_face ? (1.0 / ior) : ior;

	vec3 unit_direction = normalize(r_in.direction());

	// dot(v, n) = cos(θ)
	double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	vec3 direction;

	// if n1 / n2 * sin(θ) > 1, then total reflection, conversely refract
	if (refraction_ratio * sin_theta > 1.0 || schlick_fresnel(cos_theta, refraction_ratio) > random_double())
	{
		direction = reflect(unit_direction, rec.normal);
	}
	else
	{
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}

	srec.specular_ray = ray(rec.p, direction, r_in.time());
	return true;
}

bool oren_nayar::scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const
{
	srec.is_specular = false;
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
	return true;
}

double oren_nayar::scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
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

bool microfacet_reflection::scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const
{
	srec.is_specular = false;
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
	return true;
}

double microfacet_reflection::scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
{
	const vec3 wi = normalize(r_in.direction());
	const vec3 wo = normalize(scattered.direction());
	double Ndotwo = dot(rec.normal, wo);
	double Ndotwi = dot(rec.normal, wi);

	if (Ndotwo * Ndotwi <= 0)
		return 0;
	else
	{
		const vec3 wh = normalize(wi + wo);
		double F = schlick_fresnel(Ndotwo, ior);
		double G = distribution->G(wo, wi);
		double D = distribution->D(wh);

		printf("D = %f F = %f G = %f\n", D, F, G);
		return D * F * G / (4 * Ndotwo * Ndotwi);
	}
}
