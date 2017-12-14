#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "process.h"


void PixImage::copyOutput(PixImage src) {
	if(data)
		stbi_image_free(data);
	data = new unsigned char[src.getWidth() * src.getHeight() * src.getPchannel()];
	for(int i = 0; i < src.getWidth() * src.getPchannel() * src.getHeight(); ++i)
		data[i] = getoIndex(i);
}

void PixImage::bluring(int n) {
	if(output)
		stbi_image_free(output);
	if(n % 2 == 0)
		n++;
	create(channel);
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
