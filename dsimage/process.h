#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <cmath>
#include <stdio.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image_resize.h"
class PixImage {
private:
	int width; 
	int height;
	int channel; // rgb=3, grayscale=1
	unsigned char* data; //image information, saved line by line
	
	void setPixel(unsigned char* d, const int x, const int y, const int c, const int _width, const int pchannel, const unsigned char pix) {
		d[y * _width * pchannel + x * pchannel + c] = pix;
	}

	void setPixel(const int x, const int y, const int c, const unsigned char pix) {
		data[y * width * channel + x * channel + c] = pix;
	}

	unsigned char* create(const int _channel) {
		return new unsigned char[width * height * _channel];
	} // create data by given channel, its width and height.

	unsigned int getPixel(unsigned char* d, const int x, const int y, const int c, int pchannel) { //get pixel from any data, pchannel means the source's channel
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
		stbir_resize_uint8(data, width, height, 0, output, _width, _height, 0, 3); // resize pic by given width and height
		width = _width;
		height = _height;
		stbi_image_free(data);
		data = output;
	}

	bool readFile(const char* filename, const int pchannel = 0) {
		if(data)
			stbi_image_free(data);
		data = stbi_load(filename, &width, &height, &channel, pchannel); // read pic from file and get its width, height and channel.
		return data ? true : false;
	}

	void writeFile(const char* filename) {
		stbi_write_jpg(filename, width, height, channel, data, 70); //write pic to file by given width, height and channel. the last para means quality and 70 is enough.
	}

	void copy(PixImage& src) {
		if(data)
			stbi_image_free(data);
		width = src.width;
		height = src.height;
		channel = src.channel;
		data = create(channel);
		for(int i = 0; i < width * height * channel; ++i)
			data[i] = src.data[i];
	} // copy and paste from given PixImage.

	unsigned int getPixel(const int x, const int y, const int c) {
		return data[y*width*channel + x * channel + c];
	} // different from the private member, giePixel, the function can only get pixel from its data.

	unsigned int getIndex(const int i) {
		return data[i];
	} // get pixel by its index

	void grayscale(); // convert to grayscale

	void grayChannel(); // convert grayscale to rgb

	void bluring(int n); 

	void sobel(); 

	void combineHorizontal(PixImage* src[], int size); // horizontal stitching

};

void PixImage::bluring(int n) {
	if(n % 2 == 0)
		n++;
	int pchannel = channel;
	unsigned char* output = create(pchannel);

	for(int y = 0; y < height; ++y)
		for(int x = 0; x < width; ++x) {
			for(int c = 0; c < channel; ++c) {
				unsigned char ave;
				unsigned int sum = 0;
				int xstart = x - n / 2 < 0 ? 0 : x - n / 2;
				int xend = x + n / 2 + 1 > width ? width : x + n / 2 + 1;
				int ystart = y - n / 2 < 0 ? 0 : y - n / 2;
				int yend = y + n / 2 + 1 > height ? height : y + n / 2 + 1;
				int num = (xend - xstart) * (yend - ystart);
				for(int i = xstart; i < xend; ++i)
					for(int j = ystart; j < yend; ++j)
						sum += getPixel(data, i, j, c, channel);
				ave = sum / num;
				setPixel(output, x, y, c, width, pchannel, ave);
			}
		}
	stbi_image_free(data);
	data = output;
}

void PixImage::grayscale() {
	unsigned char* output = create(1);
	for(int x = 0; x < width; ++x)
		for(int y = 0; y < height; ++y) {
			unsigned char pix = (getPixel(x, y, 0) * 76 + getPixel(x, y, 1) * 150 + getPixel(x, y, 2) * 30) >> 8;
			setPixel(output, x, y, 0, width, 1, pix);
		}
	stbi_image_free(data);
	data = output;
	channel = 1;
}

void PixImage::grayChannel() {
	unsigned char* output = create(3);
	for(int i = 0; i < width * height; ++i)
		output[i * 3 + 0] = output[i * 3 + 1] = output[i * 3 + 2] = data[i];
	stbi_image_free(data);
	data = output;
	channel = 3;
}

void PixImage::sobel() {
	grayscale();
	unsigned int sum = 0;
	unsigned char *sob = new unsigned char[width * height], ave;
	for(int y = 1; y < height - 1; ++y)
		for(int x = 1; x < width - 1; ++x) {
			unsigned int gx = getPixel(x + 1, y - 1, 0) + 2 * getPixel(x + 1, y, 0) + getPixel(x + 1, y + 1, 0) - getPixel(x - 1, y - 1, 0) - 2 * getPixel(x - 1, y, 0) - getPixel(x - 1, y + 1, 0);
			unsigned int gy = getPixel(x - 1, y - 1, 0) + 2 * getPixel(x, y - 1, 0) + getPixel(x + 1, y - 1, 0) - getPixel(x - 1, y + 1, 0) - 2 * getPixel(x, y + 1, 0) - getPixel(x + 1, y + 1, 0);
			unsigned char g = sqrt(gx * gx + gy * gy);
			sob[y * width + x] = g;
		}
	for(int i = 1; i < width - 1; ++i) {
		sob[i] = sob[width + i];
		sob[(height - 1) * width + i] = sob[(height - 2) * width + i];
	}
	for(int i = 1; i < height - 1; ++i) {
		sob[i * width] = sob[i * width + 1];
		sob[i * width + width - 1] = sob[i * width + width - 2];
	}
	sob[0] = sob[width + 1];
	sob[width - 1] = sob[width + width - 2];
	sob[(height - 1) * width] = sob[(height - 2) * width + 1];
	sob[(height - 1) * width + width - 1] = sob[(height - 2) * width + width - 2];
	for(int i = 0; i < width * height; ++i)
		sum += sob[i];
	ave = sum / (width * height);

	int scale = 64;
	unsigned char mean = sqrt(scale * ave);
	for(int i = 0; i < width * height; ++i) {
		if(sob[i] < mean)
			sob[i] = 0;
		else
			sob[i] = 255;
	}
	stbi_image_free(data);
	data = sob;
}


void PixImage::combineHorizontal(PixImage* src[], int size) {
	int _width = width;
	for(int i = 0; i < size; ++i) {
		if(src[i]->getHeight() != height)
			src[i]->resize(src[i]->getWidth() * height / src[i]->getHeight(), height);
		if(src[i]->getChannel() == 1)
			src[i]->grayChannel();
		_width += src[i]->getWidth();
	}
	unsigned char* output = new unsigned char[_width*height*channel];
	for(int y = 0; y < height; ++y) {
		int offset = width;
		for(int x = 0; x < width; ++x)
			for(int c = 0; c < 3; ++c)
				setPixel(output, x, y, c, _width, 3, getPixel(x, y, c));
		for(int i = 0; i < size; ++i) {
			for(int x = 0; x < src[i]->getWidth(); ++x)
				for(int c = 0; c < 3; ++c)
					setPixel(output, offset + x, y, c, _width, 3, src[i]->getPixel(x, y, c));
			offset += src[i]->getWidth();
		}
	}
	stbi_image_free(data);
	data = output;
	width = _width;
	//resize(width / 2, height / 2);
}
