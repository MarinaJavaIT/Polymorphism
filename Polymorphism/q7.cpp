//7. Is it possible to access an object of the Base71 type through a pointer to a Derived71 class ?
// Accessing a Base71 object through a `Derived71*`

//The short answer: No — not safely. A `Derived71*` promises that the object it points to is 
// at least a `Derived71`. Pointing it at a plain `Base71` object breaks that promise, and using 
// it is undefined behavior.The language won't stop you at compile time (with a cast), 
// but the result is not a valid program.

//A `Derived71` object is larger than a `Base71` (it has the Base71 subobject plus its own members).
// When you access `Derived71`-only members through the pointer, you're reading/writing memory 
// that doesn't belong to the `Base71` object.

#include <iostream>

class Base71 {
public:
    virtual ~Base71() = default;
    int b = 10;
};

class Derived71 : public Base71 {
public:
    int d = 20;// extra member not present in Base71
    void show() const {
        std::cout << "b=" << b << " d=" << d << "\n";
    }
};

void main71() {
    Base71 Base71;// a real Base71 object
    Base71* bp = &Base71;

    // Attempt to treat a Base71 as a Derived71:
    Derived71* dp = static_cast<Derived71*>(bp);  // compiles! but WRONG
    dp->show();   // UB: reads dp->d, which is beyond the Base71 object
}
//The cast compiles(the types are related), but `dp-> d` accesses memory past the end of `Base71`. 
// The output is garbage, or the program crashes — this is *undefined behavior, 
// not a well - defined "partial view."

//What the compiler  will reject
//If you try it without 
// a cast, the compiler stops you, because there's no valid implicit conversion from `Base71*` 
// to `Derived71*`:

Derived71 * dp = bp;   // error: cannot convert 'Base71*' to 'Derived71*'

// What about `dynamic_cast`?

//`dynamic_cast` will correctly refuse the conversion at runtime, because the object really 
// isn't a `Derived71`:

Base71 Base71;
Base71* bp = &Base71;
Derived71* dp = dynamic_cast<Derived71*>(bp); // returns nullptr
if (!dp) {
    std::cout << "Not a Derived71 — conversion refused\n";
}

//So `dynamic_cast` protects you; `static_cast` does not.

//Accessing a Derived71 object through a Base71 pointer is fine and is the whole point 
// of polymorphism

Derived71 der;
Base71* bp = &der;     // upcast: safe, implicit
bp->someBase71Method();

//But the reverse — a `Base71` object through a `Derived71*` — is always invalid, 
// because the object simply is not a `Derived71`.

// If you genuinely need "Base71 behavior through a Derived71-typed handle"
//You can't fake it with a pointer to an unrelated object. The realistic options are:
//Store it as a `Derived71` from the start — if you need `Derived71` access, make the object a `Derived71`.
//Keep a `Base71*` and only upcast — design so you never need to pretend a `Base71` is a `Derived71`.
//Use a wrapper/interface if you need a subset of behavior — but that's a design change, not a cast.

//You cannot validly access a Base - type object through a pointer to a Derived type.
// The type system prevents it implicitly, `dynamic_cast` rejects it at runtime, 
// and forcing it with `static_cast` produces undefined behavior.