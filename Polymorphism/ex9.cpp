//Ex №9. Использование массива объектов
#include <iostream>
#include <windows.h>
#include "CQ.h"
char names[3][20] = { "низкий","средний","высокий" };
void ex9() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	Display displayMode[3];
	float width, height;
	displayMode[0].setRes(low);
	displayMode[0].setDim(640, 480);
	displayMode[1].setRes(medium);
	displayMode[1].setDim(800, 600);
	displayMode[2].setRes(high);
	displayMode[2].setDim(1600, 1200);
	std::cout << "Режимы разрешения монитора:\n";
	for (int i = 0; i < 3; i++) {
		std::cout << names[displayMode[i].getRes()] << ":";
		displayMode[i].getDim(width, height);
		std::cout << width << " x " << height << "\n";
	}
}
