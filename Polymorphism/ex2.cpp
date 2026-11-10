//Ex №2. Доступ к содержимому класса
#include <iostream>
#include <windows.h>
#include "CQ.h"
void ex2L() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Book publishBook;
	publishBook.setPages(50);//установка количества страниц
	std::cout << "Объект publishBook после вызова метода setPages(50): " << std::endl;
	std::cout << "pages = " << publishBook.getPages() << std::endl;
	std::cout << "publication = " << publishBook.publication << std::endl;//можно получить значение элемента publication можно напрямую, т.к. он является открытым
	std::cout << std::endl;
	publishBook.publication = 2020;//задать значение элемента publication можно напрямую, т.к. он является открытым
	std::cout << "Объект publishBook после установки элемента publishBook.publication = 2000:" << std::endl;
	std::cout << "pages = " << publishBook.getPages() << std::endl;
	std::cout << "publication = " << publishBook.publication << std::endl;
	std::cout << std::endl;
	publishBook.reset();
	std::cout << "Объект publishBook после вызова методы publishBook.reset():" << std::endl;
	std::cout << "pages = " << publishBook.getPages() << std::endl;
	std::cout << "publication = " << publishBook.publication << std::endl;
}
