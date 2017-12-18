#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image_resize.h"
class PixImage {
private:
	int width;
	int height;
	int channel;
	unsigned char* data;

	void setPixel(unsigned char* d, const int x, const int y, const int c, const int _width, const int pchannel, const unsigned char pix) {
		d[y * _width * pchannel + x * pchannel + c] = pix;
	}

	void setPixel(const int x, const int y, const int c, const unsigned char pix) {
		data[y * width * channel + x * channel + c] = pix;
	}

	unsigned char* create(const int _channel) {
		return new unsigned char[width * height * _channel];
	}

	unsigned int getPixel(unsigned char* d, const int x, const int y, const int c, int pchannel) {
		return d[y*width*pchannel + x * pchannel + c];
	}

public:
	PixImage() {};
	PixImage(const char* filename, const int pchannel = 0) {
		readFile(filename, pchannel);
	}

	PixImage(PixImage &src) {
		copy(src);
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	int getChannel() {
		return channel;
	}

	void resize(int _width, int _height) {
		unsigned char* output = new unsigned char[_width * _height * channel];
		stbir_resize_uint8(data, width, height, 0, output, _width, _height, 0, 3);
		width = _width;
		height = _height;
		stbi_image_free(data);
		data = output;
	}

	bool readFile(const char* filename, const int pchannel = 0) {
		if(data)
			stbi_image_free(data);
		data = stbi_load(filename, &width, &height, &channel, pchannel);
		return data ? true : false;
	}

	void writeFile(const char* filename) {
		stbi_write_jpg(filename, width, height, channel, data, 70);
	}

	void copy(PixImage& src) {
		width = src.width;
		height = src.height;
		channel = src.channel;
		data = create(channel);
		for(int i = 0; i < width * height * channel; ++i)
			data[i] = src.data[i];
	}

	unsigned int getPixel(const int x, const int y, const int c) {
		return data[y*width*channel + x * channel + c];
	}

	unsigned int getIndex(const int i) {
		return data[i];
	}

	void grayscale();

	void grayChannel();

	void bluring(int n);

	void sobel();

	void combineHorizontal(PixImage* src[], int size);

	void clear() {
		if(data)
			stbi_image_free(data);
	}
};
