#include "mainloop.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>
#undef main

int main() {
	Attrib attribs[5] = { //define GLSL shader attributes {name, index, uniform}
		{ "vertex", 0, false },
		{ "tex_coords", 1, false },
		{ "sampler", 2, false },
		{ "MVP", 3, true },
		{ "tex_samplers", 4, true }
	};

	unsigned int errors = 0;
	//allocate resource object with GL version, window information, number of units to consider the window space as, shader filenames, and attributes
	Resources *res = new Resources(4, 4, 1024, 1024, 30, 30, 256.0f, "Main Window", "vert_shader_source.txt", "frag_shader_source.txt", attribs, 5, &errors);
	if (errors > 0)
		return -1;

	mloop(res);

	delete res;

	return 0;
}