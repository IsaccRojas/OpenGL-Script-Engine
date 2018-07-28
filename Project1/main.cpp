#include "mainloop.h"
#undef main

int main() {
	Resources* res;
	Attrib attribs[4] = {
		{ "vertex", 0, false },
		{ "tex_coords", 1, false },
		{ "MVP", 2, true },
		{ "tex_sampler", 3, true }
	};
	unsigned int errors = 0;
	res = new Resources(4, 4, 400, 400, 30, 30, "Main Window", "vert_shader_source.txt", "frag_shader_source.txt", attribs, 4, &errors);
	if (errors <= 0)
		mloop(res);
	delete res;
	
	std::cout << "end";

	return 0;
}