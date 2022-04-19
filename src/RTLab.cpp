#include "RTLab.h"
#include <opencv2/opencv.hpp>

color RTLab::ray_color(const ray &r,
					   const color &background,
					   const hittable &world,
					   const hittable &lights,
					   int depth)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	scatter_record srec;
	color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
	if (!rec.mat_ptr->scatter(r, rec, srec))
		return emitted;

	if (srec.is_specular) {
		return srec.attenuation
			* ray_color(srec.specular_ray, background, world, lights, depth - 1);
	}

//	auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
//	mixture_pdf p(light_ptr, srec.pdf_ptr);
	cosine_pdf p(rec.normal);

	ray scattered = ray(rec.p, p.generate(), r.time());
	auto pdf_val = p.value(scattered.direction());

	// Monte-Carlo BRDF
	return emitted
		+ srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
			* ray_color(scattered, background, world, lights, depth - 1) / pdf_val;
}

void RTLab::reset_scene(scene&& new_scene)
{
	m_scene = std::make_shared<scene>(std::move(new_scene));
}

void RTLab::scan_calculate_color(int height, int width)
{
	int i = width, j = height;
	color pixel_color(0, 0, 0);
	auto& extent = GetExtent();
	auto& cam = GetCamera();
	for (int s = 0; s < samples_per_pixel; ++s) {
		double u = (i + random_double()) / (extent.width - 1.0);
		double v = (j + random_double()) / (extent.height - 1.0);
		ray r = cam.get_ray(u, v);
		pixel_color += ray_color(r, background, GetWorld(), GetLights(), max_depth);
	}
	//    write_color(std::cout, pixel_color, samples_per_pixel);
	write_color_table(pixel_color, j, i);
}

void RTLab::output_color(std::ostream &out, std::vector<std::vector<color>> &color_table)
{
	auto &extent = GetExtent();
	for (int j = extent.height - 1; j >= 0; --j) {
		std::cerr << "\routput remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < extent.width; ++i) {
			out_color_table(std::cout, color_table, j, i);
		}
	}
}

void RTLab::Render()
{
	auto &extent = GetExtent();

	color_table.resize(GetExtent().height + 1);
	for (auto &tab : color_table)
		tab.resize(GetExtent().width + 1);

	color_table_raw.resize(GetExtent().width * GetExtent().height);

	// Render
	std::cout << "P3\n" << extent.width << ' ' << extent.height << "\n255\n";

	omp_set_num_threads(32);

	int finish = extent.width - 1;
	static omp_lock_t lock;
	omp_init_lock(&lock);

	const auto start = std::chrono::high_resolution_clock::now();

	int key = 0;

	cv::Mat image(extent.width, extent.height, CV_8UC3);

	#pragma omp parallel for
	for (int j = extent.height - 1; j >= 0; --j) {
		for (int i = 0; i < extent.width; ++i) {
			scan_calculate_color(j, i);
		}

		#pragma omp critical
		{
			std::cerr << "\routput remaining: " << finish << ' ' << std::flush;
			finish--;
			if (key != 27)
			{
				cv::imshow("image", image);

				// j i
				for (int i = 0 ; i < extent.width ; ++i)
				{
					image.at<cv::Vec3b>(extent.height - j, i)[0] = color_table[j][i].e[0];
					image.at<cv::Vec3b>(extent.height - j, i)[1] = color_table[j][i].e[1];
					image.at<cv::Vec3b>(extent.height - j, i)[2] = color_table[j][i].e[2];
				}
				key = cv::waitKey(10);
			}
		}
	}

//	for (int j = extent.height - 1; j >= 0; --j) {
//		std::cerr << "\routput remaining: " << j << ' ' << std::flush;
//		for (int i = 0; i < extent.width; ++i) {
//			out_color_table(std::cout, color_table, j, i);
//		}
//	}

	const auto stop = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();

	std::cerr << "\n" << "duration : " << elapsed << "s\tDone.\n";
}

void RTLab::write_color_table(color pixel_color, int height, int width)
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

	color_table_raw[height * width + width].e[0] = color_table[height][width].e[0];
	color_table_raw[height * width + width].e[1] = color_table[height][width].e[1];
	color_table_raw[height * width + width].e[2] = color_table[height][width].e[2];

}