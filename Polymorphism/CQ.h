#pragma once
#include <string>
//Класс ClientsQueue
class ClientsQueue {
private:
	int* arrayOfClients;//массив для хранения элементов очереди
	int indexAdd;//индекс добавляемого элемента в очередь
	int indexRead;//индекс извлекаемого элемента из очереди
	int sizeQueue;
	int queueID;// идентификационный номер очереди
public:
	ClientsQueue();//конструктор
	ClientsQueue(int queueID, int sizeQueue);//параметризованный конструктор
	~ClientsQueue();//деструктор
	void init(int sizeQueue);
	void pushClient(int ID);
	int popClient();
};

class Book {
	int pages;//закрытое поле
public:
	int publication;//открытое поле
	//открытые методы   
	void setPages(int);
	int getPages();
	void reset();
};

class Widget {
	int xCoord;
	int yCoord;
public:
	Widget(int, int);
	void getCoords();
};
///////////////
namespace STD1 {
	class Student {
		std::string name;
	public:
		Student(std::string name);
		std::string getName();
	};
}
/////////
struct StudentS {
	int getID();//Эти элементы открыты (public) по умолчанию
	void putID(int ID);
private:
	int ID;
};
///////////
class StudentClass {
	int ID;
public:
	int getID();//Эти элементы открыты (public) по умолчанию
	void putID(int ID);
};
/////////////
namespace STD {
	class Student {
		int age;//закрытый элемент
	public:
		int getAge();
		void putAge(int age);
	};
	inline int Student::getAge() { return age; }
	inline void Student::putAge(int age) { this->age = age; }

}
///////////////////////////
enum Resolution { low, medium, high };
class Display {
	float width;
	float height;
	Resolution res;
public:
	void setDim(float width, float height) { this->width = width; this->height = height; }
	void getDim(float& width, float& height) { width = this->width; height = this->height; }
	void setRes(Resolution res) { this->res = res; }
	Resolution getRes() { return res; }
};



