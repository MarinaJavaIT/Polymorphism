//Ex №1. Использование класса ClientsQueue. 
//Программа может быть использована, например, в следующей ситуации: банк планирует ввести электронную очередь обслуживания своих клиентов, для чего в единый список клиентов, желающих получить обслуживание, будут добавляться их идентификационные номера
#include <iostream>
#include "CQ.h"
#include <windows.h>


void ex1() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	ClientsQueue legalEntity, individuals;//Создание двух объектов класса ClientsQueue
	const int LEGAL_ENTITY_SIZE = 15, INDIVIDUALS_SIZE = 10;
	legalEntity.init(LEGAL_ENTITY_SIZE);
	individuals.init(INDIVIDUALS_SIZE);
	for (int i = 1; i <= LEGAL_ENTITY_SIZE; i++)
		legalEntity.pushClient(i);
	for (int i = 1; i <= INDIVIDUALS_SIZE; i++)
		individuals.pushClient(i);
	std::cout << "Содержимое очереди юридических лиц (legalEntity): ";
	for (int i = 0; i < LEGAL_ENTITY_SIZE; i++)
		std::cout << legalEntity.popClient() << ",";
	std::cout << std::endl;
	std::cout << "Содержимое очереди физических лиц (individuals): ";
	for (int i = 0; i < INDIVIDUALS_SIZE; i++)
		std::cout << individuals.popClient() << ",";
}
