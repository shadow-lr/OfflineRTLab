#pragma once

#include "rtweekend.h"

class camera {
public:

    camera() = default;

	// vfov : vertical field-of-view in degrees

    // focus_dist : 焦距
    // aperture : 光圈直径
    camera(point3 lookfrom, point3 lookat, vec3 vup,
           double vfov, double aspect_ratio, double aperture,
           double focus_dist, double _time0 = 0, double _time1 = 0)
    {
        reset(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, _time0, _time1);
    }

    void reset(point3 lookfrom, point3 lookat, vec3 vup,
        double vfov, double aspect_ratio, double aperture,
        double focus_dist, double _time0 = 0, double _time1 = 0)
    {
		double theta = degrees_to_radians(vfov);
		double h = tan(theta / 2);
		double viewport_height = 2.0 * h;
		double viewport_width = aspect_ratio * viewport_height;
		double focal_length = 1.0;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;   // new version
		vertical = focus_dist * viewport_height * v;    // new version
		// lower_left_corner = origin - horizontal/2 - vertical/2 - w;
		// origin = lookfrom,   origin - w ==>> lookfrom - unit_vector(lookfrom - lookat)
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;    // new version
		lens_radius = aperture / 2;

		time0 = _time0;
		time1 = _time1;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, 
            random_double(time0, time1));
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    vec3 u, v, w;
    double lens_radius;
    double time0, time1;    // shutter open/close times
};

