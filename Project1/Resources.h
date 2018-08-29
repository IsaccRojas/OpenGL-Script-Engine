#pragma once
#include "gl_wrappers.h"
#include <string>
#include <vector>

class Resources
{
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event* mevent;
	GLuint vert_shader, frag_shader, program;
	int vs_length, fs_length;
	unsigned int WINDOW_WIDTH;
	unsigned int WINDOW_HEIGHT;
	float RENDER_WIDTH;
	int initGlew();
public:
	Resources(int gl_majorver, int gl_minorver, int window_w, int window_h, int window_x, int window_y, float render_width, std::string window_name, std::string vertex_shader_location, std::string fragment_shader_location, Attrib attribs[], int num_attribs, unsigned int* errors);
	~Resources();
	SDL_Window* getWindow();
	SDL_GLContext getContext();
	SDL_Event* getEvent();
	vec2 getWindowDims();
	float getRenderWidth();
	std::vector<Attrib> vAttribs;
	std::vector<unsigned int> vVertexBuffers;
	std::vector<unsigned int> vElementBuffers;
	std::vector<unsigned int> vVertexArrayObjects;
	void AddAttrib(std::string p_name, int p_index, bool p_bUniform);
	int AddNewAttrib(std::string p_name, bool p_bUniform);
};