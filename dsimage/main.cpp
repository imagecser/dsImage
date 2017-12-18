#include "process.h"

int main() {
	PixImage img("1.jpg");
	img.bluring(5);
	img.writeFile("2.jpg");
	img.sobel();
	img.writeFile("3.jpg");
	return 0;
}
