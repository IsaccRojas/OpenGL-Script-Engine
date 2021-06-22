#include "gl_wrappers.h"

template class GLBuffer<int>;
template class GLBuffer<float>;
template class GLBuffer<unsigned>;

template<class T>
GLBuffer<T>::GLBuffer(int buffer_type, T* buf_data, int size_of_data, int new_draw) {
	gen = false;
	type = buffer_type;
	draw = new_draw;
	//store data
	for (unsigned n = 0; n < size_of_data / sizeof(T); n++)
		data.push_back(buf_data[n]);
	data_ref = false;

	//generate OpenGL buffer and update it to have parameter data
	genBuffer();
	update();
}

template<class T>
GLBuffer<T>::GLBuffer(int buffer_type, std::vector<T>* buf_data, int new_draw) {
	gen = false;
	type = buffer_type;
	draw = new_draw;
	//store reference to data
	r_data = buf_data;
	data_ref = true;

	//generate OpenGL buffer and update it to have parameter data
	genBuffer();
	update();
}

template<class T>
GLBuffer<T>::GLBuffer() {
	gen = false;
	draw = 0;
	data_ref = false;
}

template<class T>
GLBuffer<T>::~GLBuffer() {}

template<class T>
void GLBuffer<T>::genBuffer() {
	//only generate new OpenGL buffer if one has not been already generated
	if (!gen) {
		glGenBuffers(1, &index);
		gen = true;
	}
}

/* FIXME: incomplete */
template<class T>
void GLBuffer<T>::delBuffer() {}

template<class T>
void GLBuffer<T>::setDraw(int new_draw) {
	draw = new_draw;
}

template<class T>
void GLBuffer<T>::bind() {
	glBindBuffer(type, index);
}

template<class T>
void GLBuffer<T>::update() {
	//bind OpenGL buffer to buffer type
	bind();

	//access data depending on if it stored via reference or not
	if (data_ref)
		glBufferData(type, r_data->size() * sizeof(T), r_data->data(), draw);
	else
		glBufferData(type, data.size() * sizeof(T), data.data(), draw);
}

/* FIXME: incomplete */
template<class T>
void GLBuffer<T>::updateSub() {}

template<class T>
unsigned int GLBuffer<T>::getIndex() {
	return index;
}

template<class T>
int GLBuffer<T>::getSize() {
	//access data depending on if it stored via reference or not
	if (data_ref)
		return r_data->size() * sizeof(T);
	else
		return data.size() * sizeof(T);
}

template<class T>
int GLBuffer<T>::getType() {
	return type;
}

template<class T>
int GLBuffer<T>::getDraw() {
	return draw;
}

template<class T>
bool GLBuffer<T>::bGen() {
	return gen;
}

template<class T>
bool GLBuffer<T>::bDataRef() {
	return data_ref;
}

int MakeSDLGLWindowContext(SDL_Window** window_handle, SDL_GLContext* context_handle, GLuint majorver, GLuint minorver, int x, int y, int w, int h, const char* title) {
	//set OpenGL attributes for subsequent SDL calls
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorver);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorver);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//create window ready for OpenGL context
	*window_handle = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (*window_handle == nullptr) {
		std::cout << "Error making window." << std::endl;
		return -1;
	}

	//create OpenGL context in window
	*context_handle = SDL_GL_CreateContext(*window_handle);
	if (*context_handle == nullptr) {
		std::cout << "Error making context." << std::endl;
		return -1;
	}
	return 0;
}

int MakeShader(GLint typeofshader, GLuint* handle, const char* shader_source) {
	//print shader source to terminal
	std::cout << "_________________________________" << std::endl;
	for (int e = 0; shader_source[e] != '\0'; e++) {
		std::cout << shader_source[e];
	}
	std::cout << "_________________________________" << std::endl;
	
	//create shader in OpenGL state machine and get handle
	*handle = glCreateShader(typeofshader);
	//provide source for shader
	glShaderSource(*handle, 1, &shader_source, NULL);
	glCompileShader(*handle);

	//check if shader compiled successfully
	int status;
	glGetShaderiv(*handle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		//report error
		int length;
		glGetShaderiv(*handle, GL_INFO_LOG_LENGTH, &length);
		char* shaderlog = new char[length];
		glGetShaderInfoLog(*handle, length, NULL, shaderlog);
		std::string t = (typeofshader == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << "Error in " << t.c_str() << " shader: " << shaderlog << std::endl;
		return -1;
	}
	return 0;
}

int MakeProgram(GLuint* handle, int numberofshaders, GLuint shaders[], std::vector<Attrib> attribs) {
	//create shader program in OpenGL state machine and get handle 
	*handle = glCreateProgram();
	
	//attach array of shader handles to program handle
	for (int l = 0; l < numberofshaders; l++)
		glAttachShader(*handle, shaders[l]);

	//bind all non-uniform attributes to program
	for (unsigned int l = 0; l < attribs.size(); l++)
		if (!attribs.at(l).bUniform)
			glBindAttribLocation(*handle, attribs.at(l).index, attribs.at(l).name.c_str());

	//link shader program
	glLinkProgram(*handle);

	//detach array of shader handles from program handle
	for (int l = 0; l < numberofshaders; l++)
		glDetachShader(*handle, shaders[l]);

	//check if program linked successfully
	int status;
	glGetProgramiv(*handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		//report error
		int length;
		glGetProgramiv(*handle, GL_INFO_LOG_LENGTH, &length);
		char* programlog = new char[length];
		glGetProgramInfoLog(*handle, length, NULL, programlog);
		std::cout << "Error linking program: " << programlog << std::endl;
		return -1;
	}
	return 0;
};

// FIXME: requires testing
int MakeBuffer(GLuint* handle, GLenum type, int amount, GLfloat* data, GLenum draw) {
	glGenBuffers(amount, handle);
	glBindBuffer(type, *handle);
	int n = 0;
	while (data[n++] != NULL);
	glBufferData(type, sizeof(GLfloat)*(n - 1), data, draw);
	return 0;
}
int MakeBuffer(GLuint* handle, GLenum type, int amount, GLuint* data, GLenum draw) {
	glGenBuffers(amount, handle);
	glBindBuffer(type, *handle);
	int n = 0;
	while (data[n++] != NULL);
	glBufferData(type, sizeof(GLuint)*(n - 1), data, draw);
	return 0;
}

GLint GetAttrib(GLuint program, const char* name) {
	GLint loc = glGetAttribLocation(program, name);
	if (loc == -1)
		std::cout << "Error getting attribute '" << name << "'." << std::endl;
	return loc;
}
GLint GetUniform(GLuint program, const char* name) {
	GLint loc = glGetUniformLocation(program, name);
	if (loc == -1)
		std::cout << "Error getting uniform '" << name << "'." << std::endl;
	return loc;
}

GLuint MakeTexture(GLint typeoftexture, Image img, int amount, GLenum active_texture_slot) {
	//set active texture
	glActiveTexture(active_texture_slot);

	//create and bind handle
	GLuint handle;
	glGenTextures(amount, &(handle));
	glBindTexture(typeoftexture, handle);

	//use data in img to load texture into OpenGL with temporarily hard-coded texture parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.img_width, img.img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return handle;
}

Image MakeTextureImage(GLint typeoftexture, const char* image_source, int amount, int texture_unit_int) {
	//set active texture
	glActiveTexture(GL_TEXTURE0 + texture_unit_int);

	//get image
	Image image;
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = SOIL_load_image(image_source, &image_width, &image_height, 0, SOIL_LOAD_RGBA);
	image.img_width = image_width;
	image.img_height = image_height;
	image.texture_unit = texture_unit_int;
	image.img_data = image_data;
	image.img_filename = image_source;

	//create and bind handle
	glGenTextures(amount, &(image.tex));
	glBindTexture(typeoftexture, image.tex);

	//use data in image to load texture into OpenGL with temporarily hard-coded texture parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.img_width, image.img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return image;
}