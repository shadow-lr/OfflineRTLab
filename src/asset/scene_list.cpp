#include "asset/scene_list.h"
#include "asset/material.h"
#include "asset/light.h"
#include "asset/scene.h"

#include "geometry/translate.h"
#include "geometry/rotate.h"

#include "shape/procedural/box.h"
#include "shape/procedural/sphere.h"
#include "shape/procedural/aarect.h"
#include "shape/procedural/flip_face.h"

#include "shape/model/triangle.h"
#include "shape/model/mesh_triangle.h"

using shape::procedural::box;
using shape::procedural::sphere;
using shape::procedural::xy_rect;
using shape::procedural::yz_rect;
using shape::procedural::xz_rect;
using shape::procedural::flip_face;

using shape::model::triangle;
using shape::model::mesh_triangle;

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

//	shared_ptr<hittable> triangle = make_shared<shape::model::triangle>(point3(190, 120, 190), point3(190, 140, 190), point3(190, 140, 220), green);
//	objects.add(triangle);

//	shared_ptr<hittable> bunny =
//		make_shared<shape::model::mesh_triangle>("assets/models/bunny.obj", green, vec3(350.0, -50.0, 50.0), vec3(1500.0, 1500.0, 1500.0));
//	objects.add(bunny);

	// lights
	hittable_list lights;
	//shared_ptr<hittable_list> lights = make_shared<hittable_list>();
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

	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent));
}

scene scene_list::test_scene()
{
	hittable_list objects;
	hittable_list lights;

	auto green = make_shared<lambertian>(color(.12, .45, .15));

	shared_ptr<object> opera_house = make_shared<shape::model::mesh_triangle>("assets/models/opera-house.obj", green);
	objects.add(opera_house);

	lights.add(make_shared<box>(point3(-115, -115, -115), point3(-85, -85, -85), shared_ptr<material>()));
	lights.add(make_shared<box>(point3(85, 85, -35), point3(115, 115, -5), shared_ptr<material>()));

	double aspect_ratio = 1.0;

	camera cam;
	cam.reset(vec3(130.0, 100.0, -200.0), vec3(0, 10, -30), vec3(0, 1, 0), 30.0, aspect_ratio, 0.0, 15.0);

	window_extent extent(1024, aspect_ratio);
	return scene(std::move(objects), std::move(lights), std::move(cam), std::move(extent));
}
