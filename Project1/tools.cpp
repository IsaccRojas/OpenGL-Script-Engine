#include "tools.h"

vec2::vec2(float p_x, float p_y) : x(p_x), y(p_y) {}
void vec2::operator=(vec2 v) {
	x = v.x;
	y = v.y;
}
void vec2::operator-=(vec2 v) {
	x -= v.x;
	y -= v.y;
}
void vec2::operator+=(vec2 v) {
	x += v.x;
	y += v.y;
}
void vec2::operator*=(float c) {
	x *= c;
	y *= c;
}
void vec2::operator/=(float c) {
	x /= c;
	y /= c;
}
vec2 vec2::operator-(vec2 v) {
	return vec2(x - v.x, y - v.y);
}
vec2 vec2::operator+(vec2 v) {
	return vec2(x + v.x, y + v.y);
}
vec2 vec2::operator*(float c) {
	return vec2(x * c, y * c);
}
vec2 vec2::operator/(float c) {
	return vec2(x / c, y / c);
}

Image::Image(int width, int height, int tex_unit, unsigned char* data, const char* filename) : img_width(width), img_height(height), texture_unit(tex_unit), img_data(data), img_filename(filename) {}
Image::Image() {
	img_width = 0;
	img_height = 0;
	texture_unit = 0;
	img_data = NULL;
	img_filename = NULL;
}

std::string getFileSource(const char* file_name, int* length) {
	std::string file_source_str, line;
	std::ifstream file(file_name);

	while (std::getline(file, line))
		file_source_str += line + "\n";
	file.close();

	*length = file_source_str.length();
	return file_source_str;
}

Image MakeImage(const char* image_source) {
	Image image;
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = SOIL_load_image(image_source, &image_width, &image_height, 0, SOIL_LOAD_RGBA);
	image.img_width = image_width;
	image.img_height = image_height;
	image.img_data = image_data;
	image.img_filename = image_source;
	
	return image;
}