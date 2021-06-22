#pragma once

#ifndef TOOLS_H_
#define TOOLS_H_

#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <SOIL.h>

// struct for implementing 2D math vector
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

//magnitude (length) of a vector
float mag(vec2 vec);

//distance between two points
float dist(vec2 vec1, vec2 vec2);

//helper class for encapsulating image data
class Image {
public:
	int img_width = 0;
	int img_height = 0;
	unsigned char* img_data;
	const char* img_filename;
	unsigned tex;
	unsigned texture_unit;
	//instantiates Image and loads it with image data using SOIL
	Image(int width, int height, int tex_unit, unsigned char* data, const char* filename);
	//instantiates empty Image
	Image();
};

/* reads and stores file_name in a returned string with each line null-terminated */
std::string getFileSource(const char* file_name, int* length);

/* loads and stores image information into a returned Image object */
Image MakeImage(const char* image_source);

#endif