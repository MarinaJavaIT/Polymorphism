//3. Which C++ language constructs relate to run - stage polymorphism ? example C++

//Runtime(Dynamic) Polymorphism in C++

//Runtime polymorphism means the decision about which function to execute is deferred until the program 
// actually runs.The compiler only knows the interface (a base type); the actual object determines the behavior

//The main C++ constructs that enable it :

//1. Inheritance + `virtual` functions (the core mechanism)
//2. Pure virtual functions / abstract classes (interfaces)
//3. Virtual destructors (correct cleanup through base pointers)
//4. `override` / `final` (safety and optimization hints)
//5. `dynamic_cast` and `typeid` (RTTI — runtime type identification)
//6. Virtual inheritance (diamond problem resolution)
//7. `std::function` / type erasure (a library - level form of runtime polymorphism)

// 1. Virtual Functions — the Core Mechanism
//A `virtual` function in a base class can be overridden in derived classes.The call is resolved at runtime 
// through a vtable (virtual table) : each object holds a hidden pointer to a table of function pointers, 
// and the actual entry is chosen based on the object's dynamic type.

#include <iostream>
#include <memory>
#include <vector>


class Animal {
public:
	// переделать переопределить override
	virtual void speak() const {          // virtual → dynamic dispatch
		std::cout << "Some animal sound\n";

	}
	virtual ~Animal() = default;           // virtual destructor (see #3)
};
class Dog : public Animal {// data types
public:
	explicit
	virtual void speak() const override { 
		std::cout << "Woof!\n"; 
	}
};
class Cat : public Animal {
public:
	void speak() const override { 
		std::cout << "Meow!\n"; }
};







int q3_0() {//smart array STL


	Animal* p;
	p = new Dog();
	p->speak();
	p = new Cat();
	p->speak();
	p = new Dog();
	p->speak();



	std::vector<std::unique_ptr<Animal>> zoo;//
	//int arr[100];
	zoo.push_back(std::make_unique<Dog>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cat>());






	for (const auto& a : zoo)//[0] [1] [2]
		a->speak();     
	
	
	
	
	// resolved at runtime: Dog→Woof, Cat→Meow
}
//** Output:**
//Woof!
//Meow!

//`a- > speak()` is a single call site, but the vtable lookup picks `Dog::speak` or `Cat::speak` 
// depending on the object's actual type — determined **when the program runs**, not when it's compiled.

// 2. Pure Virtual Functions / Abstract Classes(Interfaces)

//A pure virtual function(`= 0`) has no body in the base class; the class becomes abstract and cannot 
// be instantiated.Derived classes must implement it.This defines an interface that many implementations 
// can satisfy — the classic runtime - polymorphism contract.

class Shape {
public:
	virtual double area() const = 0;       // pure virtual → abstract class
	virtual void draw() const = 0;
	virtual ~Shape() = default;
};
class Circle : public Shape {
	double radius;
public:
	explicit Circle(double radius) : radius(radius) {}
	double area() const override { return 3.14159 * radius * radius; }
	void draw() const override { std::cout << "Circle\n"; }
};
class Square : public Shape {
	double side;
public:
	explicit Square(double side) : side(side) {}
	double area() const override { return side * side; }
	void draw() const override { std::cout << "Square\n"; }
};
// Any Shape can be passed; behavior chosen at runtime
double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
	double sum = 0;
	for (const auto& s : shapes) sum += s->area();   // virtual call
	return sum;
}
//You can add new shapes without touching `totalArea` — this is the open / closed principle in action.

// 3. Virtual Destructors
//If you delete a derived object through a base pointer, the base destructor must be virtual, otherwise 
// the derived part is never destroyed → resource leak / undefined behavior.

class Base {
public:
	virtual ~Base() = default;   // ← essential for polymorphic deletion
};

class Derived : public Base {
	int* data = new int[100];
public:
	~Derived() { delete[] data; }   // called correctly via Base
};

int q3_1() {
	Base* p = new Derived();
	delete p;    // ~Derived then ~Base — correct only because ~Base is virtual
}

// 4. `override` and `final`

//`override` — compiler verifies the function really overrides a base virtual (catches signature typos at 
// compile time).
//`final` — forbids further overriding; also lets the compiler devirtualize calls(optimization).

class Base {
public:
	virtual void f() const { std::cout << "Base\n"; }
	virtual void g() {}
};

class Derived : public Base {
public:
	void f() const override { std::cout << "Derived\n"; }  // OK: checked
	// void f() override {}    // error: not const → doesn't override
	void g() final {}          // no further overrides allowed
};

//These are not strictly part of the dispatch mechanism, but they are the standard safety tools 
// used with runtime polymorphism.

// 5. `dynamic_cast` and `typeid` (RTTI)
//Runtime Type Information lets you ask an object for its actual type at runtime — a direct consequence 
// of polymorphic classes having a vtable.
// `dynamic_cast`
//Safely converts a base pointer / reference to a derived one; returns `nullptr` (for pointers) or throws 
// `std::bad_cast` (for references) if the object isn't of that type.

#include <iostream>

void identify(Animal* a) {                     // from example #1
	if (auto* d = dynamic_cast<Dog*>(a))    // succeeds only if *a is a Dog
		std::cout << "It's a Dog!\n";
	else if (auto* c = dynamic_cast<Cat*>(a))
		std::cout << "It's a Cat!\n";
	else
		std::cout << "Unknown animal\n";
}

// `typeid`
//Returns a `std::type_info` describing the dynamic type.

#include <typeinfo>
void describe(Animal& a) {
	std::cout << "type: " << typeid(a).name() << "\n";
}
//Both require the class to be polymorphic (have at least one virtual function).

// 6. Virtual Inheritance(Diamond Problem)

//When a class inherits from two classes that share a common base, virtual inheritance ensures only one
// copy of the common base exists — resolved at runtime via virtual base pointers.

class A {
public: int x = 1;
};
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};   // only ONE A subobject

int q3_1() {
	D d;
	d.x = 42;          // unambiguous: only one A::x
	std::cout << d.x;  // 42
}
//Without `virtual`, `d.x` would be ambiguous(two copies of `A`).

// 7. Type Erasure : `std::function` and `std::any` (library - level runtime polymorphism)
//The standard library provides runtime polymorphism without inheritance via type erasure.
// `std::function`
//Holds any callable with a matching signature; the actual call is dispatched at runtime.

#include <functional>
#include <iostream>
#include <vector>
int add(int a, int b) { return a + b; }
int q3_3() {
	std::vector<std::function<int(int, int)>> ops;
	ops.push_back(add);
	ops.push_back([](int a, int b) { return a * b; });
	ops.push_back([](int a, int b) { return a - b; });

	for (auto& op : ops)
		std::cout << op(6, 3) << "\n";   // 9, 18, 3 — chosen at runtime
}

//`std::any`
//Holds a value of any type; `std::any_cast` recovers it — runtime type checking under the hood.
#include <any>
#include <iostream>
int q3_4() {
	std::any v = 42;
	v = std::string("hello");
	if (v.type() == typeid(std::string))
		std::cout << std::any_cast<std::string>(v) << "\n";
}
//This is runtime polymorphism expressed through the library rather than inheritance.

//	Common thread: in every case the compiler only sees a base/interface type; the concrete
// behavior is looked up when the program runs, typically through a vtable or an equivalent 
// mechanism.This gives flexibility(add types without changing call sites) at the cost of a small 
// indirection and a vtable pointer per object. 
//	Typical use case:when you need to treat a heterogeneous collection of objects uniformly 
// through a common interface — shapes, animals, plugins, GUI widgets, event handlers, etc.
