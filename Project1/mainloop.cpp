#include "mainloop.h"
#define DEBUG_BUFFER

int mloop(Resources* res) {
	setup(res, 512.0f);

	SOBaseScript* main_script = new SOBaseScript(res);

	while (res->getEvent()->type != SDL_QUIT && !(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])) {
		SDL_PollEvent(res->getEvent());

		main_script->base_script();
	}
	delete main_script;

	return 0;
}

int setup(Resources* res, float render_unit_size) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//SDL_GL_SetSwapInterval(1);

	render_unit_size /= 2.0f;
	glm::mat4 mModel = glm::mat4(1.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mProj = glm::ortho(-1.0f * render_unit_size, render_unit_size, -1.0f * render_unit_size, render_unit_size);
	glm::mat4 mMVP = mProj * mView * mModel;
	glUniformMatrix4fv(res->vAttribs.at(3).index, 1, GL_FALSE, glm::value_ptr(mMVP));
	GLint samplers_units[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	glUniform1iv(res->vAttribs.at(4).index, 16, samplers_units);

	return 0;
}