#include "mainloop.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>
#undef main

int main() {
	//define GLSL shader attributes {name, index, uniform}
	Attrib attribs[5] = {
		{ "vertex", 0, false },
		{ "tex_coords", 1, false },
		{ "sampler", 2, false },
		{ "MVP", 3, true },
		{ "tex_samplers", 4, true }
	};

	unsigned int errors = 0;
	//allocate resource object with SDL/GL rendering structures and handles
	Resources *res = new Resources(
		4, 4, //GL major and minor version
		1024, 1024, //SDL window dimensions
		30, 30, //SDL window position
		256.0f, //length of window, for GL rendering
		"Main Window", //window title
		"vert_shader_source.txt", //vertex shader source code
		"frag_shader_source.txt", //fragment shader source code
		attribs, 5, //array of attributes and count
		&errors
	);
	if (errors > 0)
		return -1;

	//begin main loop with allocated resources
	mloop(res);

	delete res;

	return 0;
}