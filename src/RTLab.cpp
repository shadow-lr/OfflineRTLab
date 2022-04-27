#include "RTLab.h"
//#include <opencv2/opencv.hpp>

int RTLab::thread_finish_count[thread_num + 1] = {0};

color RTLab::ray_color(const ray &r,
					   const skybox& env_skybox,
					   const object &world,
					   const object &lights,
					   int depth)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (!world.hit(r, 0.001, infinity, rec))
		return env_skybox.getBackgroundColor(r);

	scatter_record srec;
	color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
	if (!rec.mat_ptr->scatter(r, rec, srec))
		return emitted;

	const static double russian_roulette = 0.8;

	if (random_double() > russian_roulette)
		return emitted;

	if (srec.is_specular) {
		return srec.attenuation
			* ray_color(srec.specular_ray, env_skybox, world, lights, depth - 1) / russian_roulette;
	}

//	auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
//	mixture_pdf p(light_ptr, srec.pdf_ptr);
	cosine_pdf p(rec.normal);

	ray scattered = ray(rec.p, p.generate(), r.time());
	auto pdf_val = p.value(scattered.direction());

	// Monte-Carlo BRDF
	return emitted
		+ srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
			* ray_color(scattered, env_skybox, world, lights, depth - 1) / pdf_val / russian_roulette;

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
		pixel_color += ray_color(r, GetSkybox(), GetWorld(), GetLights(), max_depth);
	}
	write_color_table(pixel_color, j, i);
}

void RTLab::Render()
{
	auto &extent = GetExtent();

	resize_table();

	omp_set_num_threads(thread_num);
	const auto start = std::chrono::high_resolution_clock::now();
	auto clock_start = std::chrono::system_clock::now();

	int finish_num = 0;
	int all_num = extent.height * extent.width;
	int per_thread_num = all_num / thread_num;

	// time interval to update the progress
	float time_interval = 0.5;

	int key = 0;

	#pragma omp parallel for
	for (int j = extent.height - 1; j >= 0; --j)
	{
		for (int i = 0; i < extent.width; ++i)
		{
			scan_calculate_color(j, i);

			// process
			thread_finish_count[omp_get_thread_num()] += 1;
			#pragma omp critical
			{
				finish_num += 1;

				auto clock_now = std::chrono::system_clock::now();
				auto interval = std::chrono::duration_cast<std::chrono::seconds>(clock_now - clock_start).count();
				if (interval >= time_interval)
				{
					update_process(finish_num / (float)all_num, finish_num, all_num, per_thread_num);
					clock_start = clock_now;
				}
			}
		}
	}

	std::ofstream filestream("test-ball-temp.ppm");

	// Render
	filestream << "P3\n" << extent.width << ' ' << extent.height << "\n255\n";

	for (int j = extent.height - 1; j >= 0; --j)
	{
		std::cerr << "\routput remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < extent.width; ++i)
		{
			filestream
				<< int(color_table[j][i].e[0]) << ' '
				<< int(color_table[j][i].e[1]) << ' '
				<< int(color_table[j][i].e[2]) << '\n';
		}
	}

	filestream.close();

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

	// use for opencv
	color_table_raw[height * width + width].e[0] = color_table[height][width].e[0];
	color_table_raw[height * width + width].e[1] = color_table[height][width].e[1];
	color_table_raw[height * width + width].e[2] = color_table[height][width].e[2];

}

void RTLab::update_process(float progress, int finish_num, int all_num, int per_thread_num)
{
	system("cls");
	printf("\n");
	for (int i = 0; i < thread_num; ++i)
	{
		if (i % 4 == 0 && i != 0) printf("\n");

		printf("%3d  [", i);
		float curThreadProgress = thread_finish_count[i] / (float)per_thread_num;
		int present = int(curThreadProgress * 100.0);
		int per_present_flag = 10;
		int flag_num = present / per_present_flag;

		for (int j = 0; j < per_present_flag; ++j)
		{
			if (j < flag_num) printf("|");
			else printf(" ");
		}

		printf("%-3.1f%%]", (fmin(curThreadProgress * 100.0, 100.0)));
		printf("\t");
	}
	printf("\nAll  [");

	int total_present = int(progress * 100.0);
	int per_present_flag = 10;
	int flag_num = total_present / per_present_flag;

	for (int j = 0; j < per_present_flag; ++j)
	{
		if (j < flag_num) printf("|");
		else printf(" ");
	}

	printf("%d/%d]", finish_num, all_num);
	printf("\t");
	printf("Tasks: %d; %d running", thread_num, omp_get_num_procs());

	fflush(stdout);
}

void RTLab::resize_table()
{
	auto& extent = GetExtent();
	color_table.resize(extent.height + 1);
	for (auto &tab : color_table)
		tab.resize(extent.width + 1);

	color_table_raw.resize(extent.width * extent.height);
}
