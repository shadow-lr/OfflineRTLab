#pragma once

#include "geometry/ray.h"
#include "math/vec3.h"
#include "rtweekend.h"
#include "color.h"

#include "geometry/hittable_list.h"
#include "geometry/translate.h"
#include "geometry/rotate.h"
#include "geometry/bvh.h"
#include "geometry/pdf.h"

#include "shape/procedural/sphere.h"
#include "shape/procedural/aarect.h"
#include "shape/procedural/box.h"
#include "shape/procedural/moving_sphere.h"
#include "shape/procedural/constant_medium.h"

#include "asset/material.h"
#include "asset/camera.h"
#include "asset/noise_texture.h"
#include "asset/image_texture.h"
#include "asset/light.h"
#include "asset/scene.h"

#include "config/window_extent.h"

#include <omp.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <chrono>

class RTLab final
{
public:
	RTLab() {}
	RTLab(window_extent extent_)
	{
		color_table.resize(GetExtent().height + 1);
		for (auto &tab : color_table)
			tab.resize(GetExtent().width + 1);
	}
	RTLab(int width, int height) : RTLab(window_extent(width, height)) {}

	// Render
	void Render();
	color ray_color(const ray &r, const color &background, const hittable &world, const hittable &lights, int depth);
	void scan_calculate_color(int height, int width, color &background, int samples_per_pixel, hittable& lights);
	void output_color(std::ostream &out, std::vector<std::vector<color>> &color_table, int height, int width);

	// improve
	void reset_scene(scene&& new_scene);

	/**************Getter**************/
	const hittable_list &GetWorld() const { return m_scene->GetObjects(); }
	const hittable_list &GetLights() const { return m_scene->GetLights(); }
	const window_extent &GetExtent() const { return m_scene->GetExtent(); }
	const camera &GetCamera() const { return m_scene->GetCamera(); }

public:
	//	window_extent extent;
//	camera m_camera;

private:
//	hittable_list world;
	shared_ptr<scene> m_scene;

	int max_depth = 50;
	int samples_per_pixel = 200;
	color background = color(0, 0, 0);

	// 225 600
	std::vector<std::vector<color>> color_table;
};