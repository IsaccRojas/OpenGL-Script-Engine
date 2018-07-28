#pragma once

#ifndef GL_WRAPPERS_H_
#define GL_WRAPPERS_H_

#include <GL/glew.h>
#include <SOIL.h>
#include <SDL.h>
#include <iostream>
#include <vector>
#include "tools.h"

struct Attrib {
	std::string name;
	int index;
	bool bUniform;
};

template <class T>
class GLBuffer {
	unsigned int index;
	int type;
	int draw;
	bool gen;
	bool data_ref;
public:
	GLBuffer(int buffer_type, T* buf_data, int size_of_data, int new_draw);
	GLBuffer(int buffer_type, std::vector<T>* buf_data, int new_draw);
	GLBuffer();
	~GLBuffer(); /* FIXME: incomplete */
	void genBuffer();
	void delBuffer(); /* FIXME: incomplete */
	void createBuffer(/* ... */); /* FIXME: incomplete */
	void setDraw(int new_draw);
	void bind();
	void update();
	void updateSub(); /* FIXME: incomplete */
	unsigned int getIndex();
	int getSize();
	int getType();
	int getDraw();
	bool bGen();
	bool bDataRef();
	std::vector<T> data;
	std::vector<T>* r_data;
};

/* returns -1 if invalid, 0 if valid --REQUIRES OPENGL INITIALIZED */
int MakeSDLGLWindowContext(SDL_Window** window_handle, SDL_GLContext* context_handle, GLuint majorver, GLuint minorver, int x, int y, int w, int h, const char* title);

/* returns -1 if invalid, 0 if valid --REQUIRES OPENGL INITIALIZED */
int MakeShader(GLint typeofshader, GLuint* handle, const char* shader_source);

/* returns -1 if invalid, 0 if valid --REQUIRES OPENGL INITIALIZED */
int MakeProgram(GLuint* handle, int numberofshaders, GLuint shaders[], std::vector<Attrib> attribs);

/* returns -1 if invalid, 0 if valid; buffer remains binded --REQUIRES OPENGL INITIALIZED */
int MakeBuffer(GLuint* handle, GLenum type, int amount, GLuint* data, GLenum draw);
/* returns -1 if invalid, 0 if valid; buffer remains binded --REQUIRES OPENGL INITIALIZED */
int MakeBuffer(GLuint* handle, GLenum type, int amount, GLfloat* data, GLenum draw);

/* returns -1 if invalid, the correct location otherwise --REQUIRES OPENGL INITIALIZED */
GLint MakeAttrib(GLuint program, const char* name);
/* returns -1 if invalid, the correct location otherwise --REQUIRES OPENGL INITIALIZED */
GLint MakeUniform(GLuint program, const char* name);

/* returns -1 if invalid, index if valid; texture remains binded --REQUIRES OPENGL INITIALIZED*/
GLuint MakeTexture(GLint typeoftexture, Image img, int amount);
/* returns -1 if invalid, index if valid; texture remains binded --REQUIRES OPENGL INITIALIZED*/
Image MakeTextureImage(GLint typeoftexture, const char* image_source, int amount);

#endif