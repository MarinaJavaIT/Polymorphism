//10. What is a virtual method ?

//A virtual method is a member function declared with the `virtual` keyword that supports
// dynamic dispatch: when you call it through a Base101 - class pointer or reference, 
// C++ calls the version belonging to the object's actual (dynamic) type — not the version 
// belonging to the pointer's static type.

//This is how C++ implements runtime polymorphism.

#include <iostream>
class Base101 {
public:
    virtual ~Base101() = default;// virtual destructor (see below)
    virtual void speak() const {// virtual → dynamic dispatch
        std::cout << "Base101 speaking\n";
    }
    void normal() const {// NOT virtual → static dispatch
        std::cout << "Base101 normal\n";
    }
};

class Derived101 : public Base101 {
public:
    void speak() const override {// 'override' checks the signature
        std::cout << "Derived101 speaking\n";
    }
};

void q101() {
    Base101  b;
    Derived101 d;

    Base101* p1 = &b;
    Base101* p2 = &d;=// upcast: a Derived101 is-a Base101

    p1->speak();=// "Base101 speaking"
    p2->speak();=// "Derived101 speaking"  ← dynamic dispatch

    p2->normal();=// "Base101 normal"  ← static: resolved at compile time
}

//Even though `p2` has static type `Base101*`, the call `p2-> speak()` runs 
// `Derived101::speak`, because `speak` is virtual and the object is really a `Derived101`

// How it works(briefly)
// For each class with virtual functions, the compiler builds a vtable (a table of function pointers)
// Every object of that class carries a hidden vptr pointing to its class's vtable.
// A virtual call goes through the object's vptr at runtime → the correct override is found.
// Non - virtual calls are resolved at compile time, with no runtime overhead.

// `override` and `final`
//`override` (C++11) tells the compiler "this is meant to override a Base virtual." 
// If the signature doesn't match, you get a compile error. 
// Always use it — it catches subtle bugs (wrong `const`, wrong parameter types).
// `final` prevents further overriding(on a method) or further derivation(on a class)

class Derived102 : public Base101 {
public:
    void speak() const override final { ... }   // must match Base101::speak
};

//Note: `virtual` appears only in the Base101 declaration.
// In Derived classes the function is virtual automatically — you can repeat `virtual` 
// but it's not required

// Why you need a virtual destructor

//If you `delete` a Derived object through a Base pointer, the destructor must be virtual, 
// otherwise only the Base destructor runs → the Derived part leaks.

class Base101 {
public:
    virtual ~Base101() = default;// essential when deleting via Base101*
};

Base101* p = new Derived101();
delete p;// runs ~Derived101 then ~Base101 — only if virtual

// The classic "slicing" gotcha
//Virtual dispatch works through pointers and references.
// Passing by value copies only the Base part and loses the Derived type

void f(Base101 b) { b.speak(); }   // slices → "Base speaking"
void g(Base101& b) { b.speak(); }   // reference → "Derived speaking"

Derived101 d;
f(d);   // Base speaking  (sliced copy)
g(d);   // Derived speaking
```
// Pure virtual → abstract class
//A method with `= 0` is pure virtual: it has no implementation in the Base, 
// and the class becomes abstract(cannot be instantiated).
// Derived classes must override it to be instantiable

class Shape101 {
public:
    virtual ~Shape101() = default;
    virtual double area() const = 0;// pure virtual → Shape101 is abstract
};

class Circle101 : public Shape101 {
    double r;
public:
    explicit Circle101(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
};

// Shape101 s;            // error: abstract class
Shape101* s = new Circle101(2.0);   // OK: polymorphism via Base pointer
std::cout << s->area();        // Circle101::area
delete s;

//Key points:
//Declare `virtual` in the Base; Derived classes override it(mark with `override`).
//Dispatch happens when called through a Base pointer or reference.
//Always give a Base class with virtual functions a virtual destructor if it may be deleted polymorphically.
//Use pure virtual (`= 0`) to define interfaces/abstract classes.