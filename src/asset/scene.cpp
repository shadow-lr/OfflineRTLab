#include "asset/scene.h"

scene::scene(hittable_list &objects, hittable_list &lights, camera &cam, window_extent &extent)
{
}

scene::scene(hittable_list &&objects_, hittable_list &&lights_, camera &&cam_, window_extent &&extent_) :
	objects(std::move(objects_)), lights(std::move(lights_)), cam(std::move(cam_)), extent(extent_)
{
}

scene::scene(scene &&other) noexcept:
	objects(std::move(other.objects)), lights(std::move(other.lights)), cam(std::move(other.cam)), extent(other.extent)
{
}
