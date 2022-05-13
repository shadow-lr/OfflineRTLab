#pragma once

#include "math/vec3.h"

class microfacet_distribution
{
public:
	virtual double D(const vec3& wh) const = 0;
	double G(const vec3& wo, const vec3& wi) const;
	double pdf_val(const vec3& wo, const vec3& wi) const;
protected:
	microfacet_distribution() {}
};