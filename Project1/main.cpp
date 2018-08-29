#include "mainloop.h"
#undef main

int main() {
	Resources* res;
	Attrib attribs[5] = {
		{ "vertex", 0, false },
		{ "tex_coords", 1, false },
		{ "sampler", 2, false },
		{ "MVP", 3, true },
		{ "tex_samplers", 4, true }
	};
	unsigned int errors = 0;
	res = new Resources(4, 4, 1024, 1024, 30, 30, 256.0f, "Main Window", "vert_shader_source.txt", "frag_shader_source.txt", attribs, 5, &errors);
	if (errors <= 0)
		mloop(res);
	delete res;
	
	std::cout << "end";

	return 0;
}