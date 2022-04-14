#include "RTLab.h"
#include "asset/scene_list.h"

int main()
{
	RTLab lab;

	lab.reset_scene(scene_list::cornell_box());
	lab.Render();
}