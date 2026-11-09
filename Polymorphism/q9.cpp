//9. Can a method parameter that has a reference type to a Base91 class accept objects of a Derived91 class ?
// Passing a Derived91 object to a Base91 - class reference parameter

// Yes. A parameter of type `Base91&` (or `const Base91 & `) can bind to an object of any 
// class Derived91 from `Base91`. This is the reference version of an upcast, and it's the 
// foundation of polymorphism in C++.

#include <iostream>
class Base91 {
    public:
        virtual ~Base91() = default;
        virtual void whoAmI() const { std::cout << "Base91\n"; }
};

class Derived91 : public Base91 {
public:
    void whoAmI() const override { std::cout << "Derived91\n"; }
};

class Derived912 : public Base91 {
public:
    void whoAmI() const override { std::cout << "Derived912\n"; }
};

// Parameter is a reference to Base91
void printType(Base91& b) {     // accepts Base91 AND anything Derived91 from it
    b.whoAmI();               // virtual dispatch → calls the real type's version
}

void q91() {
    Base91     b;
    Derived91  d;
    Derived912 d2;

    printType(b);    // Base91
    printType(d);    // Derived91   ← Derived91& binds to Base91& (upcast)
    printType(d2);   // Derived912
}
// Why it works

//A `Derived91` object contains a `Base91` subobject.
// A `Base91&` can refer to that subobject, and because the referenced function is `virtual`, 
// the call is dispatched to the actual dynamic type.This is exactly the same upcast rule as 
// `Derived91*` → `Base91*`, just in reference form :

Derived91 d;
Base91& rb = d;     // implicit, safe — no cast needed
Base91* pb = &d;    // same thing with pointers

//`const Base& ` — the common case

//Taking a `const Base& ` is especially useful because it also accepts temporaries and rvalues,
// and signals "I won't modify the object":

void inspect(const Base91 & b) { b.whoAmI(); }   // read-only

inspect(Derived91{});// temporary Derived91 binds to const Base91&
inspect(d);// lvalue Derived91 binds too

//A plain `Base&` (non - const) will not bind to a temporary, only to lvalues.

//The important caveat: it stays a `Base&`
//Inside the function, the parameter's static type is `Base&`. 
// You can only call `Base`'s members directly.
// To reach `Derived`-only members you'd need a downcast — and only if the object really is that type

void maybeDerived91(Base91 & b) {
    b.whoAmI();                              // fine: Base91 member
    // b.Derived91Only();                      // error: not in Base91
    if (auto* d = dynamic_cast<Derived91*>(&b)) // checked downcast
        d->Derived91Only();                     // safe
}
//Also note: by - reference avoids slicing. Passing by value(`void f(Base b)`) would copy only
// the `Base` part and lose the Derived state — references and pointers don't have that problem

