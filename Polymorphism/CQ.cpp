
#include <iostream>
#include "CQ.h"
using namespace STD;


//Определение конструктора
ClientsQueue::ClientsQueue() {
	indexAdd = indexRead = 0;
	sizeQueue = 20;
	arrayOfClients = new int[sizeQueue];
	queueID = 0;
	std::cout << "Очередь инициализирована.\n";
}
//Определение параметризированного конструктора
ClientsQueue::ClientsQueue(int queueID, int sizeQueue) {
	arrayOfClients = new int[sizeQueue];
	indexAdd = indexRead = 0;
	this->queueID = queueID;
	this->sizeQueue = sizeQueue;
	std::cout << "Очередь №" << this->queueID << " инициализирована.\n";
}

// Определение деструктора
ClientsQueue::~ClientsQueue() {
	delete[] arrayOfClients;
	std::cout << "Очередь разрушенa.\n";
}

//реализация метода init() класса ClientsQueue
void ClientsQueue::init(int sizeQueue) {
	indexRead = indexAdd = 0;
	this->sizeQueue = sizeQueue;
	arrayOfClients = new int[sizeQueue];
}
//метод занесения в очередь целочисленного значения
void ClientsQueue::pushClient(int ID) {
	if (indexAdd == sizeQueue) {
		std::cout << "Очередь заполнена.\n";
		return;
	}
	arrayOfClients[indexAdd++] = ID;
}
//Метод извлечения из очереди целочисленного значения
int ClientsQueue::popClient() {
	if (indexRead == indexAdd) {
		std::cout << "Очередь пуста.\n";
		return 0;
	}
	return arrayOfClients[indexRead++];
}

void Book::setPages(int numberOfPag) {
	pages = numberOfPag;//прямое обращение к pages
	publication = 2025;//прямое обращение к publication
}
int Book::getPages() {
	return pages;//прямое обращение к переменной pages
}
void Book::reset() {
	setPages(0); 	//вызов метода setPages()
	publication = 2025;
}
/////////////////
//Передаем 2 аргумента конструктору класса Widget()
Widget::Widget(int xCoord, int yCoord) {
	this->xCoord = xCoord; this->yCoord = yCoord;
}
void Widget::getCoords() {
	std::cout << "coordinate x:" << xCoord << std::endl;
	std::cout << "coordinate y:" << yCoord << std::endl;
}
////////////

void StudentS::putID(int ID) { this->ID = ID; }
int StudentS::getID() { return ID; }

STD1::Student::Student(std::string name) { this->name = name; }
std::string STD1::Student::getName() { return name; }
/////////
int StudentClass::getID() { return ID; }
void StudentClass::putID(int ID) { this->ID = ID; }
/////////////


