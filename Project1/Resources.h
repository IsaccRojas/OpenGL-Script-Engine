#pragma once
#include "gl_wrappers.h"
#include <string>
#include <vector>

/*
class for initialization and access of SDL2/OpenGL I/O data structures and handles.
*/
class Resources
{
	//SDL window, context, event instances/handles
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event* mevent;

	//internal GL shader and program indices
	GLuint vert_shader, frag_shader, program;
	int vs_length, fs_length;

	unsigned int WINDOW_WIDTH;
	unsigned int WINDOW_HEIGHT;
	float RENDER_WIDTH;
	
	//initializes and error-checks Glew
	int initGlew();
public:
	/*
	gl_majorver, gl_minorver: major and minor version numbers of OpenGL to be used
	window_w, window_h: window width and height
	window_x, window_y: window position
	render_width: length of window, for GL rendering
	window_name: window name
	vertex_shader_location: vertex shader source code location
	fragment_shader_location: fragment shader source code location
	attribs: array of attribute data
	num_attribs: number of attributes in array
	error: error handle
	*/
	Resources(int gl_majorver, int gl_minorver, int window_w, int window_h, int window_x, int window_y, float render_width, std::string window_name, std::string vertex_shader_location, std::string fragment_shader_location, Attrib attribs[], int num_attribs, unsigned int* errors);
	~Resources();

	SDL_Window* getWindow();
	SDL_GLContext getContext();
	SDL_Event* getEvent();
	vec2 getWindowDims();
	float getRenderWidth();
	std::vector<Attrib> vAttribs;

	//std::vector<unsigned int> vVertexBuffers;
	//std::vector<unsigned int> vElementBuffers;
	//std::vector<unsigned int> vVertexArrayObjects;
	
	//add existing shader attribute to internally stored vector of attributes
	void AddAttrib(std::string p_name, int p_index, bool p_bUniform);
	//add new shader attribute to internally stored vector of attributes
	int AddNewAttrib(std::string p_name, bool p_bUniform);
};