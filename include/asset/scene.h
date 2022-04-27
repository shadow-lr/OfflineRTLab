#pragma once

#include "geometry/hittable_list.h"
#include "config/window_extent.h"
#include "camera.h"
#include "asset/skybox.h"

class scene final {
public:
	scene() {}
	scene(scene&& scene) noexcept;

	scene(hittable_list &&objects_, hittable_list &&lights_, camera &&cam_, window_extent &&extent_, std::unique_ptr<skybox>&& env_skybox_);

	const hittable_list &GetObjects() const { return objects; }
	const hittable_list &GetLights() const { return lights; }
	const camera &GetCamera() const { return cam; }
	const window_extent &GetExtent() const { return extent; }
	const skybox &GetSkybox() const { return *env_skybox; }

private:
	hittable_list lights;
	hittable_list objects;
	camera cam;
	window_extent extent;
	std::unique_ptr<skybox> env_skybox;
};