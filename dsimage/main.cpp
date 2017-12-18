#include "process.h"

int main() {
	PixImage src("1.jpg"), blu(src), sob(src);
	blu.bluring(5);
	sob.sobel();
	PixImage* a[2] = {&blu, &sob };
	src.combineHorizontal(a, 2);
	src.writeFile("2.jpg");
	//sob.writeFile("2.jpg");
	return 0;
}
