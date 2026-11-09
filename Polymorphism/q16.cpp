//16. How does multiple inheritance differ from single inheritance ?
// Multiple vs.single inheritance in C++

//Single inheritance : **a class has exactly one direct base class.
// Multiple inheritance : **a class has two or more direct base classes and inherits 
// the members(and interface) of all of them.

//C++ supports both; many other OO languages(Java, C#) allow only single inheritance 
// of classes(though they permit multiple interface implementation).

// Single inheritance — one base

#include <iostream>

class Animal161 {
public:
	virtual ~Animal161() = default;
	void breathe() const { std::cout << "breathing\n"; }
};

class Dog161 : public Animal161 {          // exactly one base
public:
	void bark() const { std::cout << "Woof\n"; }
};

void q161() {
	Dog161 d;
	d.breathe();   // inherited from Animal161
	d.bark();      // own
}

//`Dog161` is - a `Animal161`. Simple, unambiguous hierarchy.

// Multiple inheritance — several bases

#include <iostream>

class Swimmer {
public:
	virtual ~Swimmer() = default;
	void swim() const { std::cout << "swimming\n"; }
};

class Flyer {
public:
	virtual ~Flyer() = default;
	void fly() const { std::cout << "flying\n"; }
};

// Duck is both a Swimmer and a Flyer
class Duck : public Swimmer, public Flyer {
public:
	void quack() const { std::cout << "Quack\n"; }
};

void q162() {
	Duck d;
	d.swim();    // from Swimmer
	d.fly();     // from Flyer
	d.quack();   // own
}
//`Duck` inherits the combined interface and implementation of both `Swimmer` and `Flyer`.

// The ambiguity problem

//If two bases have a member with the same name, an unqualified use is ambiguous — you must qualify it :

class A {
public: void hello() const { std::cout << "A\n"; }
};
class B {
public: void hello() const { std::cout << "B\n"; }
};

class C : public A, public B {};

void q163() {
	C c;
	// c.hello();          // ERROR: ambiguous — A::hello or B::hello?
	c.A::hello();          // "A" — disambiguate explicitly
	c.B::hello();          // "B"
}

// The diamond problem

//A classic multiple - inheritance pitfall : if two bases share a common base,
// the derived object ends up with** two copies** of that common base — 
// leading to duplication and ambiguity.

class Person {
public: int id = 0;
};
class Student : public Person {};
class Employee : public Person {};

class TeachingAssistant : public Student, public Employee {};
// TeachingAssistant has TWO Person subobjects!

void q164() {
	TeachingAssistant ta;
	// ta.id = 5;                    // ERROR: ambiguous (two Person copies)
	ta.Student::id = 1;             // one copy
	ta.Employee::id = 2;             // the other copy
}
// Fix: virtual inheritance
//Declaring the shared base as** virtual** makes all paths share a** single** subobject :

class Person {
public: int id = 0;
};
class Student : virtual public Person {};
class Employee : virtual public Person {};

class TeachingAssistant : public Student, public Employee {};
// now only ONE Person subobject

void 165() {
	TeachingAssistant ta;
	ta.id = 5;      // OK — unambiguous, single shared Person
}

//Virtual inheritance solves the diamond but adds layout / complexity overhead — use it deliberately.

// The most common good use : interfaces

//Multiple inheritance is especially clean for combining** abstract interfaces** (classes of pure virtual functions) :

class Serializable {
public:
	virtual ~Serializable() = default;
	virtual std::string serialize() const = 0;
};

class Comparable {
public:
	virtual ~Comparable() = default;
	virtual bool lessThan(const Comparable&) const = 0;
};

// Implements two independent interfaces + a real base
class Record : public Serializable, public Comparable {
public:
	std::string serialize() const override { return "record"; }
	bool lessThan(const Comparable&) const override { return false; }
};

//This "one concrete base + several interface bases" pattern is the safest and most idiomatic use of multiple inheritance.

// Summary

// Single inheritance : one base — simple, unambiguous, the default choice.
// Multiple inheritance : several bases — powerful for mixing independent capabilities or interfaces, but brings** name ambiguity**, ** object layout complexity**, and the** diamond problem** (fixed with `virtual` inheritance).
// Prefer single inheritance of implementation + multiple inheritance of interfaces * *to keep designs clear.
// If two bases define the same name, qualify it(`obj.Base::member`); if they share a common base, consider `virtual` inheritance.

// Bottom line: multiple inheritance lets one class combine the interfaces/implementations of several bases, whereas single inheritance follows one chain.It's more flexible but requires care to avoid ambiguity and duplicated base subobjects.