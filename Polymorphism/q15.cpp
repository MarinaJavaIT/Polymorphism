//15. What is a pure virtual function ?
// Pure virtual functions in C++

//A pure virtual function is a virtual member function declared with `= 0` instead of a body

virtual ReturnType name(params) = 0;

//It tells the compiler : "This class declares this operation as part of its interface, 
// but provides no implementation here — every concrete Derived152 class must supply one."
//A class containing at least one pure virtual function is an abstract class: 
// it cannot be instantiated.
// It exists only to define an interface that Derived152 classes implement.

#include <iostream>
class Shape151 {                         // abstract: has a pure virtual function
public:
	virtual ~Shape151() = default;
	virtual double area() const = 0;  // pure virtual — no body here
};

class Circle151 : public Shape151 {
	double r;
public:
	explicit Circle151(double r) : r(r) {}
	double area() const override { return 3.14159 * r * r; }   // must implement
};

class Square151 : public Shape151 {
	double s;
public:
	explicit Square151(double s) : s(s) {}
	double area() const override { return s * s; }
};

void q151() {
	// Shape151 s;// ERROR: cannot instantiate abstract class
	Shape151* Shape151s[] = { new Circle151(2), new Square151(3) };

	for (Shape151* s : Shape151s)
		std::cout << s->area() << "\n";   // 12.5664, 9 — dynamic dispatch

	for (Shape151* s : Shape151s) delete s;
}
//- `Shape151` cannot be instantiated — it's abstract.
//- `Circle151` and `Square151` * *must * *override `area()` to be concrete; if one doesn't, it too remains abstract.

// What makes a class abstract
//Any class with at least one pure virtual function(that isn't overridden somewhere below) is abstract:
class Abstract1 {
public:
	virtual void f() = 0;// abstract
};

class Abstract2 : public Abstract1 {
	// doesn't override f → still abstract
};

class Concrete : public Abstract2 {
public:
	void f() override {}               // now concrete
};

// Abstract1 a1;    // error
// Abstract2 a2;    // error
Concrete c;         // OK
//Can a pure virtual function still have a body ?

//Yes!A pure virtual function may have an implementation — 
//it's just that it's still pure(the class is still abstract) and Derived152 classes must still 
//override it. A Derived152 class can call the Base152 implementation explicitly

class Base152 {
public:
	virtual void greet() const = 0;    // pure virtual, but has a body...
};

void Base152::greet() const {             // ...defined out of line
	std::cout << "Hello from Base152\n";
}

class Derived152 : public Base152 {
public:
	void greet() const override {
		Base152::greet();                 // explicitly call the Base152 body
		std::cout << "Hello from Derived152\n";
	}
};

//This is useful for providing default shared behavior that Derived classes can opt into — 
// but they still must override to make the class concrete.

//Summary
//A pure virtual function is declared `= 0` and has no(required) implementation.
//It makes the class abstract — you cannot create objects of that type.
//Any concrete Derived class must override every pure virtual function it inherits.
//A pure virtual function can have a body, callable via `Base::func()` from Derived classes.
//Pure virtual functions are how C++ expresses interfaces and enforces a contract on Derived classes.
//In one line: a pure virtual function says "every Derived type must provide this operation, but there's no sensible default here" * — turning the Base152 class into an abstract interface rather than something you can instantiate.