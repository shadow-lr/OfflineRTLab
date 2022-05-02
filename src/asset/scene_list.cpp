#include "asset/scene_list.h"
#include "asset/material.h"
#include "asset/light.h"
#include "asset/scene.h"
#include "asset/image_texture.h"
#include "asset/skybox.h"
#include "asset/hdr_texture.h"

#include "geometry/translate.h"
#include "geometry/rotate.h"

#include "shape/procedural/box.h"
#include "shape/procedural/sphere.h"
#include "shape/procedural/aarect.h"
#include "shape/procedural/flip_face.h"
#include "shape/procedural/cube.h"
#include "shape/procedural/square.h"
#include "shape/procedural/cylinder.h"
#include "shape/procedural/disk.h"
#include "shape/procedural/cone.h"
#include "shape/procedural/hyperboloid.h"
#include "shape/procedural/paraboloid.h"

#include "shape/model/triangle.h"
#include "shape/model/mesh_triangle.h"

using shape::procedural::box;
using shape::procedural::sphere;
using shape::procedural::xy_rect;
using shape::procedural::yz_rect;
using shape::procedural::xz_rect;
using shape::procedural::flip_face;
using shape::procedural::cube;
using shape::procedural::square;
using shape::procedural::cylinder;
using shape::procedural::disk;
using shape::procedural::cone;
using shape::procedural::hyperboloid;
using shape::procedural::paraboloid;

using shape::model::triangle;
using shape::model::mesh_triangle;

scene scene_list::book2_final_scene()
{
	return scene();
}

scene scene_list::cornell_box()
{
	// objects
	hittable_list objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));

	//    auto baseColor = vec3(.34299999, .54779997, .22700010);
	auto baseColor = vec3(1.0);
	auto light = make_shared<diffuse_light>(baseColor * color(15, 15, 15));

	// box
	shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
	shared_ptr<object> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	auto glass = make_shared<dielectric>(1.5);
	objects.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

	// wall
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
	//    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	// lights
	hittable_list lights;
	lights.add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
	lights.add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

	// camera(default Camera)
	point3 lookfrom = point3(278, 278, -800);
	point3 lookat = point3(278, 278, 0);
	vec3 vup(0, 1, 0);
	double dist_to_focus = 10.0;
	double vfov = 40.0;
	double aperture = 0.0;
	double aspect_ratio = 1.0;

	window_extent extent(1024, aspect_ratio);

	// move to camera
	camera cam;
	cam.reset(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	std::unique_ptr<skybox> env_skybox = std::make_unique<constant_skybox>(color(0, 0, 0));

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

scene scene_list::test_scene()
{
	hittable_list objects;
	hittable_list lights;

	auto green = make_shared<lambertian>(color(.12, .45, .15));

	shared_ptr<object> opera_house = make_shared<mesh_triangle>("assets/models/opera-house.obj", green);
	objects.add(opera_house);

	lights.add(make_shared<box>(point3(-115, -115, -115), point3(-85, -85, -85), shared_ptr<material>()));
	lights.add(make_shared<box>(point3(85, 85, -35), point3(115, 115, -5), shared_ptr<material>()));

	double aspect_ratio = 1.0;

	camera cam;
	cam.reset(vec3(130.0, 100.0, -200.0), vec3(0, 10, -30), vec3(0, 1, 0), 30.0, aspect_ratio, 0.0, 15.0);

	window_extent extent(1024, aspect_ratio);

	std::unique_ptr<skybox> env_skybox = std::make_unique<constant_skybox>(color(0, 0, 0));

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

scene scene_list::test_ball()
{
	hittable_list objects, lights;

	auto grey = make_shared<lambertian>(color(.8, .8, .8));
	auto stand = make_shared<lambertian>(color(.2, .2, .2));
	auto checker_tex = make_shared<checker_texture>(color(0, 0, 0), color(1, 1, 0));
	auto checker_surface = make_shared<lambertian>(checker_tex);

	auto glass = make_shared<dielectric>(1.5);
	auto metal_sphere = make_shared<metal>(color(0.8, 0.8, 0.9), 0.0);

	shared_ptr<object> mesh001 =
		make_shared<mesh_triangle>("assets/models/test-ball/Mesh001.obj", grey, vec3(0.0571719, 0.213656, 0.0682078), vec3(0.482906));
	shared_ptr<object> mesh002 =
		make_shared<mesh_triangle>("assets/models/test-ball/Mesh002.obj", metal_sphere, vec3(0.156382, 0.777229, 0.161698), vec3(0.482906));
	shared_ptr<object> mesh000 =
		make_shared<mesh_triangle>("assets/models/test-ball/Mesh000.obj", stand, vec3(0.110507, 0.494301, 0.126194), vec3(0.482906));
	shared_ptr<object> mesh050 = make_shared<mesh_triangle>("assets/models/test-ball/Mesh050.obj", checker_surface);

	objects.add(mesh001);
	objects.add(mesh002);
	objects.add(mesh000);
	objects.add(mesh050);

	double aspect_ratio = 1.0;

	camera cam;
	cam.reset(vec3(3.04068,  3.17153, 3.20454), vec3(-0.67, -0.32, -0.65), vec3(0, 1, 0), 30.0, aspect_ratio, 0.0, 1.0);

	window_extent extent(800, aspect_ratio);

	std::unique_ptr<skybox> env_skybox = std::make_unique<constant_skybox>(color(1, 1, 1));

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

scene scene_list::dark1()
{
	hittable_list objects, lights;

	auto ground_tex = make_shared<image_texture>("assets/textures/marble.jpg");

	objects.add(make_shared<square>(vec3(10, 0, 10), vec3(-10, 0, 10), vec3(-10, 0, -10), vec3(10, 0, -10), make_shared<
		lambertian>(ground_tex)));
	objects.add(make_shared<square>(vec3(100, 0, 95), vec3(0, 0, -5), vec3(0, 100, -5), vec3(100, 100, 95), make_shared<
		metal>(vec3(0.7, 0.6, 0.5), 0.0)));
	objects.add(make_shared<square>(vec3(-100, 0, 95), vec3(-100, 100, 95), vec3(0, 100, -5), vec3(0, 0, -5), make_shared<
		metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	auto checker = make_shared<checker_texture>(vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));
	objects.add(make_shared<cube>(vec3(-0.5, 0.8, -1.5), 1.5, make_shared<lambertian>(checker))); // or vec3(-0.5 0.75 -1.5)

	auto mat2_tex = make_shared<image_texture>("assets/textures/jupiter_map.jpg");
	objects.add(make_shared<sphere>(vec3(-0.5, 2.5, -1.5), 1.0, shared_ptr<material>(new dielectric(1.5))));

	objects.add(make_shared<cylinder>(2.0, 90, -2.0, 2.0, make_shared<lambertian>(checker)));

	auto checker2 = make_shared<checker_texture>(vec3(1, 0, 0), vec3(0.9, 0.9, 0.9));
	objects.add(make_shared<cylinder>(0.5, 30, -2.0, 2.0, make_shared<lambertian>(checker2)));

	objects.add(make_shared<disk>(2.0, 1.5, 0.5, 180.0, make_shared<lambertian>(checker)));

	objects.add(make_shared<cone>(5.0, 1.0, 360.0, make_shared<lambertian>(mat2_tex)));

	// light
	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	objects.add(make_shared<flip_face>(make_shared<cube>(vec3(0.4, 6, 1.0), 2.5, light)));

	double aspect_ratio = 16.0 / 9.0;

	camera cam;
	cam.reset(vec3(4, 6, 15), vec3(0, 1, 0), vec3(0, 1, 0), 20.0, aspect_ratio, 0.0, 15.0);

	window_extent extent(1024, aspect_ratio);
	std::unique_ptr<skybox> env_skybox = std::make_unique<constant_skybox>(color(0, 0, 0));

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

scene scene_list::shape_show()
{
	hittable_list objects, lights;

	auto ground_tex = make_shared<image_texture>("assets/textures/marble.jpg");
	auto mat2_tex = make_shared<image_texture>("assets/textures/jupiter_map.jpg");
	auto guyu_tex = make_shared<image_texture>("assets/textures/guyu.jpg");

	objects.add(make_shared<square>(vec3(10, 0, 10), vec3(-10, 0, 10), vec3(-10, 0, -10), vec3(10, 0, -10), make_shared<
		lambertian>(ground_tex)));
	objects.add(make_shared<square>(vec3(100, 0, 95), vec3(0, 0, -5), vec3(0, 100, -5), vec3(100, 100, 95), make_shared<
		metal>(vec3(0.7, 0.6, 0.5), 0.0)));
	objects.add(make_shared<square>(vec3(-100, 0, 95), vec3(-100, 100, 95), vec3(0, 100, -5), vec3(0, 0, -5), make_shared<
		metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	auto checker = make_shared<checker_texture>(vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));
	objects.add(make_shared<translate>(make_shared<disk>(2.0, 1.5, 0.5, 300.0, make_shared<lambertian>(guyu_tex)), vec3(-1.5, 1.5, 0)));

	shared_ptr<object> cylinder1 = make_shared<cylinder>(1, 360, -2.0, 2.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0));
	cylinder1 = make_shared<rotate_x>(cylinder1, 90.0);
	cylinder1 = make_shared<translate>(cylinder1, vec3(2.0, 1.5, 0));
	objects.add(cylinder1);

	objects.add(make_shared<rotate_x>(make_shared<paraboloid>(1.0, -2.0, 3.0, 360.0, make_shared<lambertian>(checker)), 90.0));

	shared_ptr<object> hyper1 = make_shared<hyperboloid>(vec3(0,0,0), vec3(1.25, 1.25, 1.25), 360.0, make_shared<lambertian>(checker));
	hyper1 = make_shared<rotate_x>(hyper1, 90.0);
	hyper1 = make_shared<translate>(hyper1, vec3(3.0, 0, 3));
	objects.add(hyper1);

	// light
	auto light = make_shared<diffuse_light>(color(5, 5, 5));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(-4, 4, -4, 4, 6, light)));

	double aspect_ratio = 16.0 / 9.0;
	window_extent extent(800, aspect_ratio);

	camera cam;
	cam.reset(vec3(4, 6, 15), vec3(0, 1, 0), vec3(0, 1, 0), 20.0, aspect_ratio, 0.0, 15.0);

	std::unique_ptr<skybox> env_skybox = std::make_unique<constant_skybox>(color(0, 0, 0));
	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

scene scene_list::skybox_show()
{
	// skybox
	hittable_list objects, lights;
	objects.add(make_shared<sphere>(vec3(0, 1, 0), 2.0, make_shared<dielectric>(1.5)));

	double aspect_ratio = 16.0 / 9.0;

	camera cam;
	cam.reset(vec3(0, 3, 10), vec3(0, 1, 0), vec3(0, 1, 0), 40, aspect_ratio, 0.02, 15.0);

	window_extent extent(800, aspect_ratio);

	std::unique_ptr<skybox> env_skybox = std::make_unique<hdr_skybox>(make_shared<hdr_texture>("assets/HDRs/sunset.hdr"));

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent), std::move(env_skybox));
}

