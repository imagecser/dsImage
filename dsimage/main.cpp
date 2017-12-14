#include "process.h"

int main() {
	PixImage img("1.jpg");
	//img.bluring(5);
	img.grayscale();
	img.writeFile("2.jpg");
	return 0;
}
