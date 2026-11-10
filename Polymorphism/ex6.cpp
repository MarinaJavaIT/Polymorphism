#define _HAS_STD_BYTE 0
#include "CQ.h"
#include <string>
#include <iostream>
#include <windows.h>
using namespace STD1;

void ex6L() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string studentName = "Анастасия";
	Student obFirst = studentName;//вызов метода Student("Анастасия")
	studentName = "Ангелина";
	Student obSecond = studentName;//вызов метода Student("Ангелина")
	studentName = "Александра";
	Student obThird = studentName;//вызов метода Student("Александра")
	std::cout << "имя первого объекта:" << obFirst.getName() << std::endl;
	std::cout << "имя второго объекта:" << obSecond.getName() << std::endl;
	std::cout << "имя третьего объекта:" << obThird.getName() << std::endl;
}

