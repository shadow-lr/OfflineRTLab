#pragma once

#include "texture.h"

class image_texture : public texture
{
public:
	// per pixel 3 bits
	const static int bytes_per_pixel = 3;

	image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

	image_texture(const char *filename);

	~image_texture() { delete data; data = nullptr; }

	virtual color value(double u, double v, const vec3 &p) const override;

private:
	unsigned char *data;
	int width, height;
	int bytes_per_scanline;
};