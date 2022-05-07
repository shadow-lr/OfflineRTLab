#pragma once

#include "geometry/ray.h"
#include "math/vec3.h"
#include "rtweekend.h"

#include "geometry/hittable_list.h"
#include "geometry/translate.h"
#include "geometry/rotate.h"
#include "geometry/bvh.h"
#include "geometry/pdf.h"

#include "asset/material.h"
#include "asset/camera.h"
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
	RTLab() : RTLab(800, 600) {}
	RTLab(window_extent extent_)
	{
		color_table.resize(extent_.height + 1);
		for (auto &tab : color_table)
			tab.resize(extent_.width + 1);
		color_table_raw.resize(extent_.width * extent_.height);
	}
	RTLab(int width, int height) : RTLab(window_extent(width, height)) {}

	// Render
	void Render();
	color ray_color(const ray &r, const skybox& env_skybox, const object &world, const object &lights, int depth);
	void scan_calculate_color(int height, int width);

	void write_color_table(color pixel_color, int height, int width);

	void output2file();
	void resize_table();
	void update_process(float progress, int finish_num, int all_num, int per_thread_num);

	// Improve
	void reset_scene(scene &&new_scene);

	void set_output_name(const char* name) { output_name = name; }

	// Getter
	const hittable_list &GetWorld() const { return m_scene->GetObjects(); }
	const hittable_list &GetLights() const { return m_scene->GetLights(); }
	const window_extent &GetExtent() const { return m_scene->GetExtent(); }
	const camera &GetCamera() const { return m_scene->GetCamera(); }
	const skybox &GetSkybox() const { return m_scene->GetSkybox(); }

private:
	shared_ptr<scene> m_scene;

	static const int thread_num = 12;
	static int thread_finish_count[thread_num + 1];

	// render
	int max_depth = 10;
	int samples_per_pixel = 300;

	std::vector<std::vector<color>> color_table;
	std::vector<color> color_table_raw;

	std::string output_name = "output.ppm";
};