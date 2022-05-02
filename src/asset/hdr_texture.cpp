#include "asset/hdr_texture.h"

hdr_texture::hdr_texture(const char *filename)
{
	data = HDRLoader::load(filename);
	if (data == nullptr)
	{
		std::cerr << "failed to open hdr texture\n";
	}
}

color hdr_texture::value(double u, double v, const point3 &p) const
{
	// If we have no texture data, then return solid cyan as a debugging aid.
	if (data == nullptr)
		return color(0, 1, 1);

	// Clamp input texture coordinates to [0,1] x [0,1]
	u = clamp(u, 0.0, 1.0);
	v = clamp(v, 0.0, 1.0);

	int width = data->width;
	int height = data->height;

	auto i = static_cast<int>(u * width);
	auto j = static_cast<int>(v * height);

	// Clamp integer mapping, since actual coordinates should be less than 1.0
	if (i >= width) i = width - 1;
	if (j >= height) j = height - 1;

	auto &cols = data->cols;
	int index = j * width * 3 + i * 3;

	return color(cols[index], cols[index + 1], cols[index + 2]);
}
