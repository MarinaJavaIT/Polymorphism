#include <iostream>
#include <windows.h>
#include "CQ.h"

void ex3L() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ClientsQueue legalEntity, individuals;//Создание двух объектов класса ClientsQueue
	const int LEGAL_ENTITY_SIZE = 10, INDIVIDUALS_SIZE = 15;
	for (int i = 1; i <= LEGAL_ENTITY_SIZE; i++)
		legalEntity.pushClient(i);
	for (int i = 1; i <= INDIVIDUALS_SIZE; i++)
		individuals.pushClient(i);
	std::cout << "Содержимое очереди физических лиц (legalEntity): ";
	for (int i = 0; i < LEGAL_ENTITY_SIZE; i++)
		std::cout << legalEntity.popClient() << ",";
	std::cout << std::endl;
	std::cout << "Содержимое очереди юридических лиц (individuals): ";
	for (int i = 0; i < INDIVIDUALS_SIZE; i++)
		std::cout << individuals.popClient() << ",";
	std::cout << std::endl;
}

