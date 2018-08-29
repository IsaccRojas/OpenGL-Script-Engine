#include "Resources.h"

Resources::Resources(int gl_majorver, int gl_minorver, int window_w, int window_h, int window_x, int window_y, float render_width, std::string window_name, std::string vertex_shader_location, std::string fragment_shader_location, Attrib attribs[], int num_attribs, unsigned int* errors) {
	*errors = 0;
	WINDOW_WIDTH = window_w;
	WINDOW_HEIGHT = window_h;
	RENDER_WIDTH = render_width;
	if (MakeSDLGLWindowContext(&window, &context, gl_majorver, gl_minorver, window_x, window_y, WINDOW_WIDTH, WINDOW_HEIGHT, window_name.c_str()) == -1) {
		std::cout << "Errors initializing window and/or context." << std::endl;
		(*errors)++;
	}
	initGlew();
	if (MakeShader(GL_VERTEX_SHADER, &vert_shader, getFileSource(vertex_shader_location.c_str(), &vs_length).c_str()) == -1) {
		std::cout << "Errors initializing vertex shader." << std::endl;
		(*errors)++;
	}
	if (MakeShader(GL_FRAGMENT_SHADER, &frag_shader, getFileSource(fragment_shader_location.c_str(), &fs_length).c_str()) == -1) {
		std::cout << "Errors initializing fragment shader." << std::endl;
		(*errors)++;
	}

	for (int n = 0; n < num_attribs; n++) {
		vAttribs.push_back(attribs[n]);
	}

	GLuint shaders[2] = { vert_shader, frag_shader };
	if (MakeProgram(&program, 2, shaders, vAttribs) == -1) {
		std::cout << "Errors initializing program." << std::endl;
		(*errors)++;
	}
	glUseProgram(program);

	mevent = new SDL_Event;

	std::cout << std::endl << *errors << " error(s) during initialization of resources object." << std::endl;
	if (*errors > 0)
		getchar();
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

Resources::~Resources() {
	SDL_DestroyWindow(window);
	vAttribs.clear();
	delete mevent;
}

int Resources::initGlew() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	else
		return 0;
}

SDL_Window* Resources::getWindow() {
	return window;
}

SDL_GLContext Resources::getContext() {
	return context;
}

SDL_Event* Resources::getEvent() {
	return mevent;
}

vec2 Resources::getWindowDims() {
	return vec2(float(WINDOW_WIDTH), float(WINDOW_HEIGHT));
}

float Resources::getRenderWidth() {
	return RENDER_WIDTH;
}

void Resources::AddAttrib(std::string p_name, int p_index, bool p_bUniform) {
	Attrib attr;
	attr.name = p_name;
	attr.index = p_index;
	attr.bUniform = p_bUniform;
	vAttribs.push_back(attr);
}

int Resources::AddNewAttrib(std::string p_name, bool p_bUniform) {
	Attrib attr;
	attr.name = p_name;
	attr.bUniform = p_bUniform;
	if (!attr.bUniform)
		attr.index = MakeAttrib(program, attr.name.c_str());
	else
		attr.index = MakeUniform(program, attr.name.c_str());
	if (attr.index == -1) {
		std::cout << "Errors initializing attribute/uniform '" << attr.name << "'." << std::endl;
		return -1;
	}
	else
		vAttribs.push_back(attr);
	return 0;
}