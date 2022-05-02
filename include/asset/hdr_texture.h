#pragma once

#include "texture.h"
#include "utility/HDRLoader.hpp"

class hdr_texture : public texture
{
public:
	hdr_texture(const char *filename);
	color value(double u, double v, const point3 &p) const override;

public:
	HDRData* data;
};