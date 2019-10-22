#include "mainloop.h"
#define DEBUG_BUFFER

//main loop
int mloop(Resources* res) {
	setup(res);

	int err = 0;
	SOBaseScript* main_script = new SOBaseScript(res, &err); //define new base script (root script for engine to run under)
	if (err != 0) {
		std::cout << "ERROR: errors constructing SOBaseScript" << std::endl;
		delete main_script;
		return -1;
	}
	while (res->getEvent()->type != SDL_QUIT && !(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])) {
		SDL_PollEvent(res->getEvent()); //poll for events

		main_script->base_script();
	}
	delete main_script;

	return 0;
}

//set up rendering environment
int setup(Resources* res) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	SDL_GL_SetSwapInterval(1); //enable VSync

	float render_unit_size = res->getRenderWidth() / 2.0f; //let GL units be window units * 2

	//set model matrix to identity matrix
	glm::mat4 mModel = glm::mat4(1.0f);
	//set view matrix with lookAt(eye, center, up)
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//set projection matrix to orthogonal perspective, capturing square region defined by render_unit_size
	glm::mat4 mProj = glm::ortho(-1.0f * render_unit_size, render_unit_size, -1.0f * render_unit_size, render_unit_size);
	//full transformation matrix
	glm::mat4 mMVP = mProj * mView * mModel;

	//assign previously defined matrix uniform attribute to mMVP
	glUniformMatrix4fv(res->vAttribs.at(3).index, 1, GL_FALSE, glm::value_ptr(mMVP));
	//define list of "texture indices" to sample fragment data from
	GLint samplers_units[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	//assign previously defined array uniform attribute to sampler_units
	glUniform1iv(res->vAttribs.at(4).index, 16, samplers_units);

	return 0;
}