#include "RTLab.h"
#include "asset/scene_list.h"

int main()
{
	RTLab lab;

	lab.reset_scene(scene_list::dark1());
	lab.set_output_name("dark1_hdr_skybox_test.png");
	lab.Render();
}