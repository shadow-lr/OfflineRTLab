#pragma once

struct window_extent {
	int width, height;
	window_extent() {}
	window_extent(int width_, int height_) : width(width_), height(height_) {}
	window_extent(int width, double aspect_ratio) : width(width), height(static_cast<int>(width / aspect_ratio)) {}
	window_extent(window_extent &extent) : width(extent.width), height(extent.height) {}
};