#include "tools.h"

vec2::vec2(float p_x, float p_y) : x(p_x), y(p_y) {}
vec2::vec2() {}

Image::Image(int width, int height, unsigned char* data, const char* filename) : img_width(width), img_height(height), img_data(data), img_filename(filename) {}
Image::Image() {
	img_width = 0;
	img_height = 0;
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