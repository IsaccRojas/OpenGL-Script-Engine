#pragma once

#ifndef TOOLS_H_
#define TOOLS_H_

#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <SOIL.h>

struct vec2 {
	vec2(float x = 0.0f, float y = 0.0f);
	void operator=(vec2 v);
	void operator-=(vec2 v);
	void operator+=(vec2 v);
	void operator*=(float c);
	void operator/=(float c);
	vec2 operator-(vec2 v);
	vec2 operator+(vec2 v);
	vec2 operator*(float c);
	vec2 operator/(float c);
	float x;
	float y;
};

float mag(vec2 vec);

float dist(vec2 vec1, vec2 vec2);

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