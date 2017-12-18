#include "process.h"
#include <thread>

#define SUM 4
#define NUM 2

void process(int start, int step) {
	PixImage src, blu, sob;
	char filename[20];
	for(int i = start; i < start + step; ++i) {
		printf("%d\n", i);
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
}

int main() {
	int piece = SUM / NUM;
	std::thread t[NUM];
	for(int i = 0; i < NUM; ++i)
		t[i] = std::thread(process, i * piece + 1, piece);
	for(int i = 0; i < NUM; ++i)
		t[i].join();
	return 0;
}
