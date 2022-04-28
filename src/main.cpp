#include "RTLab.h"
#include "asset/scene_list.h"

int main()
{
	RTLab lab;

	lab.reset_scene(scene_list::dark1());
	lab.Render();
}