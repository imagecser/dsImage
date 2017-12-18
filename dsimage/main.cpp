#include "process.h"

int main() {
	PixImage src, blu, sob;
	char filename[10];
	for(int i = 1; i < 5; ++i) {
		sprintf(filename, "%d.jpg", i);
		src.readFile(filename); blu.copy(src); sob.copy(src);
		blu.bluring(5);
		sob.sobel();
		PixImage* a[2] = { &blu, &sob };
		src.combineHorizontal(a, 2);
		sprintf(filename, "%dds.jpg", i);
		src.writeFile(filename);
		blu.clear(); sob.clear();
	}
	return 0;
}
