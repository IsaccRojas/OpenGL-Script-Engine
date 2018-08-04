#pragma once

#ifndef TOOLS_H_
#define TOOLS_H_

#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <SOIL.h>

struct vec2 {
	vec2(float x, float y);
	vec2();
	float x;
	float y;
};

class Image {
public:
	int img_width = 0;
	int img_height = 0;
	unsigned char* img_data;
	const char* img_filename;
	unsigned tex;
	unsigned texture_unit;
	Image(int width, int height, int tex_unit, unsigned char* data, const char* filename);
	Image();
};

/* reads and stores file_name in a returned string with each line null-terminated */
std::string getFileSource(const char* file_name, int* length);

/* loads and stores image information into a returned Image object */
Image MakeImage(const char* image_source);

#endif