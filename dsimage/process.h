#pragma once
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

class PixImage {
private:
	int width;
	int height;
	int channel;
	int pchannel;
	unsigned char* data;
	unsigned char* output;

	void setPixel(const int x, const int y, const int c, const unsigned char pix) {
		output[y * width * pchannel + x * pchannel + c] = pix;
	}

	bool create(const int _channel) {
		pchannel = _channel;
		output = new unsigned char[width * height * pchannel];
		return output ? true : false;
	}

public:
	PixImage() {};
	PixImage(const char* filename, const int pchannel = 0) {
		readFile(filename, pchannel);
	}
	~PixImage() {
		stbi_image_free(data);
		if(output)
			stbi_image_free(output);
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

	int getPchannel() {
		return pchannel;
	}

	bool readFile(const char* filename, const int pchannel = 0) {
		data = stbi_load(filename, &width, &height, &channel, pchannel);
		return data ? true : false;
	}

	void writeFile(const char* filename) {
		if(output)
			stbi_write_jpg(filename, width, height, pchannel, output, 100);
		else
			stbi_write_jpg(filename, width, height, channel, data, 100);
	}


	unsigned int getiPixel(const int x, const int y, const int c) {
		return data[y * width *channel + x * channel + c];
	}

	unsigned int getoIndex(const int i) {
		return output[i];
	}

	void copyOutput(PixImage src);

	void bluring(int n);

	void grayscale();
};
