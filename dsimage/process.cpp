#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "process.h"


void PixImage::copyOutput(PixImage src) {
	if(data)
		stbi_image_free(data);
	width = src.getWidth();
	height = src.getHeight();
	channel = src.getPchannel();
	data = new unsigned char[width * height  * channel];
	for(int i = 0; i < width * height * channel; ++i)
		data[i] = src.getoIndex(i);
}

void PixImage::bluring(int n) {
	if(output)
		stbi_image_free(output);
	if(n % 2 == 0)
		n++;
	create(output, channel);
	for(int x = 0; x < width; ++x)
		for(int y = 0; y < height; ++y) {
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
						sum += getiPixel(i, j, c);
				ave = sum / num;
				setPixel(x, y, c, ave);
			}
		}
}

void PixImage::grayscale() {
	if(output)
		stbi_image_free(output);
	create(1);
	for(int x = 0; x < width; ++x)
		for(int y = 0; y < height; ++y) {
			unsigned char pix = (getiPixel(x, y, 0) * 76 + getiPixel(x, y, 1) * 150 + getiPixel(x, y, 2) * 30) >> 8;
			setPixel(x, y, 0, pix);
		}
}

void PixImage::sobel() {
	grayscale();
	copyOutput(*this);
	unsigned int sum = 0;
	unsigned char *sob = new unsigned char[width * height], ave;
	for(int x = 1; x < width - 1; ++x)
		for(int y = 1; y < height - 1; ++y) {
			unsigned int gx = getiPixel(x + 1, y - 1, 0) + 2 * getiPixel(x + 1, y, 0) + getiPixel(x + 1, y + 1, 0) - getiPixel(x - 1, y - 1, 0) - 2 * getiPixel(x - 1, y, 0) - getiPixel(x - 1, y + 1, 0);
			unsigned int gy = getiPixel(x - 1, y - 1, 0) + 2 * getiPixel(x, y - 1, 0) + getiPixel(x + 1, y - 1, 0) - getiPixel(x - 1, y + 1, 0) - 2 * getiPixel(x, y + 1, 0) - getiPixel(x + 1, y + 1, 0);
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
	
	int scale = 3;
	unsigned char mean = sqrt(scale * ave);
	for(int i = 0; i < width * height; ++i) {
		if(sob[i] < mean)
			output[i] = 0;
		else
			output[i] = 255;
	}
}
