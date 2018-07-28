#include "gl_wrappers.h"

template class GLBuffer<int>;
template class GLBuffer<float>;
template class GLBuffer<unsigned>;

template<class T>
GLBuffer<T>::GLBuffer(int buffer_type, T* buf_data, int size_of_data, int new_draw) {
	gen = false;
	type = buffer_type;
	draw = new_draw;
	for (unsigned n = 0; n < size_of_data / sizeof(T); n++)
		data.push_back(buf_data[n]);
	data_ref = false;
	genBuffer();
	update();
}

template<class T>
GLBuffer<T>::GLBuffer(int buffer_type, std::vector<T>* buf_data, int new_draw) {
	gen = false;
	type = buffer_type;
	draw = new_draw;
	r_data = buf_data;
	data_ref = true;
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
	if (!gen) {
		glGenBuffers(1, &index);
		gen = true;
	}
}

template<class T>
void GLBuffer<T>::delBuffer() {}

template<class T>
void GLBuffer<T>::createBuffer() {}

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
	bind();
	if (data_ref)
		glBufferData(type, r_data->size() * sizeof(T), r_data->data(), draw);
	else
		glBufferData(type, data.size() * sizeof(T), data.data(), draw);
}

template<class T>
void GLBuffer<T>::updateSub() {}

template<class T>
unsigned int GLBuffer<T>::getIndex() {
	return index;
}

template<class T>
int GLBuffer<T>::getSize() {
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorver);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorver);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	*window_handle = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (*window_handle == nullptr) {
		std::cout << "Error making window." << std::endl;
		return -1;
	}
	*context_handle = SDL_GL_CreateContext(*window_handle);
	if (*context_handle == nullptr) {
		std::cout << "Error making context." << std::endl;
		return -1;
	}
	return 0;
}

int MakeShader(GLint typeofshader, GLuint* handle, const char* shader_source) {
	std::cout << "_________________________________" << std::endl;
	for (int e = 0; shader_source[e] != NULL; e++) {
		std::cout << shader_source[e];
	}
	std::cout << "_________________________________" << std::endl;
	*handle = glCreateShader(typeofshader);
	glShaderSource(*handle, 1, &shader_source, NULL);
	glCompileShader(*handle);

	int status;
	glGetShaderiv(*handle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
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
	*handle = glCreateProgram();

	std::string sources;
	int len = 0, type = 0;
	
	for (int l = 0; l < numberofshaders; l++)
		glAttachShader(*handle, shaders[l]);

	for (unsigned int l = 0; l < attribs.size(); l++)
		if (!attribs.at(l).bUniform)
			glBindAttribLocation(*handle, attribs.at(l).index, attribs.at(l).name.c_str());

	glLinkProgram(*handle);

	for (int l = 0; l < numberofshaders; l++)
		glDetachShader(*handle, shaders[l]);

	int status;
	glGetProgramiv(*handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
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
	for (int e = 0; data[e] != NULL; e++) { n++; }
	glBufferData(type, sizeof(GLfloat)*n, data, draw);
	return 0;
}
int MakeBuffer(GLuint* handle, GLenum type, int amount, GLuint* data, GLenum draw) {
	glGenBuffers(amount, handle);
	glBindBuffer(type, *handle);
	int n = 0;
	for (int e = 0; data[e] != NULL; e++) { n++; }
	glBufferData(type, sizeof(GLuint)*n, data, draw);
	return 0;
}

GLint MakeAttrib(GLuint program, const char* name) {
	GLint loc = glGetAttribLocation(program, name);
	if (loc == -1)
		std::cout << "Error getting attribute '" << name << "'." << std::endl;
	return loc;
}
GLint MakeUniform(GLuint program, const char* name) {
	GLint loc = glGetUniformLocation(program, name);
	if (loc == -1)
		std::cout << "Error getting uniform '" << name << "'." << std::endl;
	return loc;
}

GLuint MakeTexture(GLint typeoftexture, Image img, int amount) {
	GLuint handle;
	glGenTextures(amount, &(handle));
	glBindTexture(typeoftexture, handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.img_width, img.img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return handle;
}

Image MakeTextureImage(GLint typeoftexture, const char* image_source, int amount) {
	Image image;
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = SOIL_load_image(image_source, &image_width, &image_height, 0, SOIL_LOAD_RGBA);
	image.img_width = image_width;
	image.img_height = image_height;
	image.img_data = image_data;
	image.img_filename = image_source;

	glGenTextures(amount, &(image.tex));
	glBindTexture(typeoftexture, image.tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.img_width, image.img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return image;
}