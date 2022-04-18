#pragma once

#include "math/vec3.h"
#include "rtweekend.h"

#include <iostream>
#include <opencv2/openv>

static void write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

    // Divide the color by the number of samples
    // pixel_color
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

static void write_color_table(color pixel_color, int samples_per_pixel, std::vector<std::vector<color>> &color_table, int height, int width)
{
	double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples
    // pixel_color
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    color_table[height][width].e[0] = 256 * clamp(r, 0.001, 0.999);
    color_table[height][width].e[1] = 256 * clamp(g, 0.001, 0.999);
    color_table[height][width].e[2] = 256 * clamp(b, 0.001, 0.999);
}

static void out_color_table(std::ostream &out, std::vector<std::vector<color>> &color_table, int height, int width)
{
	out << static_cast<int>(color_table[height][width].e[0]) << ' '
		<< static_cast<int>(color_table[height][width].e[1]) << ' '
		<< static_cast<int>(color_table[height][width].e[2]) << '\n';
}

