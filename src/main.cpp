#include "RTLab.h"
#include "asset/scene_list.h"

int main()
{
	RTLab lab;

	lab.reset_scene(scene_list::dragon());
	lab.set_output_name("oren-nayer-test-real-0.png");
	lab.Render();
}