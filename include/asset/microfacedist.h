#pragma once

#include "math/vec3.h"
#include "math/mathinline.h"

class microfacet_distribution
{
public:
	static double RoughnessToAlpha(double roughness)
	{
		roughness = std::max(roughness, (double)1e-3);
		double x = std::log(roughness);
		return 1.62142f + 0.819955f * x + 0.1734f * x * x +
			0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
	}

	virtual double D(const vec3 &wh) const = 0;
	virtual double lambda(const vec3 &w) const = 0;
	double G1(const vec3& w) const
	{
		return (1 / (1 + lambda(w)));
	}
	double G(const vec3 &wo, const vec3 &wi) const
	{
		return (1 / (1 + lambda(wo) + lambda(wi)));
	}
	double pdf_val(const vec3 &wo, const vec3 &wi) const;
protected:
	microfacet_distribution() {}
};

class beckmann_distribution : public microfacet_distribution
{
public:
	beckmann_distribution(double alpha_x, double alpha_y) : alphax(alpha_x), alphay(alpha_y) {}

	double D(const vec3 &wh) const override
	{

	}
protected:
	const double alphax, alphay;
};

class trowbridge_reitz_distribution : public microfacet_distribution
{
public:
	trowbridge_reitz_distribution(const double alpha_x, const double alpha_y) : alphax(alpha_x), alphay(alpha_y) {}
	double D(const vec3 &wh) const override
	{
		double tan2Theta = Tan2Theta(wh);
		if (std::isinf(tan2Theta)) return 0;
		const double cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
		double e = (Cos2Phi(wh) / (alphax * alphax) +
			Sin2Phi(wh) / (alphay * alphay)) * tan2Theta;

		return 1 / (PI * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
	}

	double lambda(const vec3 &w) const override
	{
		double absTanTheta = std::abs(TanTheta(w));
		if (std::isinf(absTanTheta))
			return 0.0;

		double alpha = std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
		double alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
		return((-1 + std::sqrt(1.0f + alpha2Tan2Theta)) / 2);
	}

protected:
	const double alphax, alphay;
};