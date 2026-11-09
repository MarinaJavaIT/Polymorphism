//11. Is it necessary to redefine(redo) the virtual method of the Base class in each Derived111 class ?
// Must every Derived111 class redefine a virtual method ?
//No. Redefining(overriding) a virtual method in a Derived111 class is optional — except when 
// the Base method is pure virtual (`= 0`), in which case the Derived111 class must override it 
// to be instantiable

//Case 1 : Ordinary(non - pure) virtual — override is optional
//If the Base provides a body, any Derived111 class that doesn't override simply Inherits111 the Base version

#include <iostream>
class Base111 {
public:
	virtual ~Base111() = default;
	virtual void speak() const { std::cout << "Base111 speaking\n"; }
};

class Overrides111 : public Base111 {
public:
	void speak() const override { std::cout << "Overrides111 speaking\n"; }
};

class Inherits111 : public Base111 {
	// no speak() — Inherits111 Base111::speak
};

void q111() {
	Base111      b;   b.speak();// Base111 speaking
	Overrides111 o;   o.speak();// Overrides111 speaking
	Inherits111  i;   i.speak();// Base111 speaking  ← inherited

	Base111* p = &i;
	p->speak();// Base111 speaking (dynamic type is Inherits111,
	//  which has no override → uses Base111's)
}
// Case 2: Pure virtual — override is required to instantiate
//A pure virtual method(`= 0`) makes the class abstract.
// Any concrete Derived111 class must verride it; otherwise that Derived111 class is abstract too.
class Shape111 {
public:
	virtual ~Shape111() = default;
	virtual double area() const = 0;      // pure virtual
};

class Circle111 : public Shape111 {
public:
	double area() const override { return 3.14 * 2 * 2; }// must override
};

class NoArea : public Shape111 {
	// no area() — NoArea is still abstract
};

// Circle111 c;    // OK: concrete
// NoArea n;    // error: abstract class — area() not overridden

//Case 3: Partial hierarchy — some levels override, some don't
//An intermediate class can override, and deeper classes can inherit that override :

class Animal111 {
public:
	virtual ~Animal111() = default;
	virtual void sound() const { std::cout << "...\n"; }
};

class Dog111 : public Animal111 {
public:
	void sound() const override { std::cout << "Woof\n"; }
};

class Puppy111 : public Dog111 {
	// Inherits111 Dog111::sound
};

Puppy111 p;  p.sound();   // Woof  (from Dog111)

// Even overriding doesn't "erase" the Base111 version
// A Derived111 class can override and still call the Base implementation explicitly via `Base::method()`

class Derived111 : public Base111 {
public:
	void speak() const override {
		Base111::speak();                 // run Base111 version
		std::cout << "Derived111 extra\n";
	}
};

//And an abstract Base can provide a body for a pure virtual method that Derived classes may call

class Base111 {
public:
	virtual void f() = 0;// pure virtual, but...
};
void Base111::f() { /* default body */ }  // ...can still have a definition

//Bottom line: overriding is only mandatory for pure virtual methods.
// For an ordinary virtual method, a Derived class may override it — and usually should 
// if it needs different behavior — but if it doesn't, 
// it simply Inherits and uses the Base implementation.