#include "process.h"
#include <stdlib.h>

void process(int start, int step) {
	PixImage src, blu, sob;
	char filename[20];
	for(int i = start; i < start + step; ++i) {
		printf("%d\n", i);
		sprintf(filename, "file/%d.jpg", i);
		src.readFile(filename); blu.copy(src); sob.copy(src);
		blu.bluring(5);
		sob.sobel();
		PixImage* a[2] = { &blu, &sob };
		src.combineHorizontal(a, 2);
		sprintf(filename, "ds/%dds.jpg", i);
		src.writeFile(filename);
		blu.clear(); sob.clear();
	}
}

int main(int argc, char** argv) {
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	process(start, end);
	return 0;
}
