//6. Is it possible to convert a base class pointer to the type of a pointer to a derived class? 
// Converting a `Base*` to a `Derived*` in C++

//Yes — this is called downcasting, and C++ gives you several ways to do it.
#include <iostream>
#include <stdexcept>

class Base61 {
public:
	virtual ~Base61() = default;
	virtual void whoAmI() const { std::cout << "Base\n"; }
};
class Derived61 : public Base61 {
public:
	void whoAmI() const override { std::cout << "Derived61\n"; }
	void derivedOnly() const { std::cout << "Derived61-only method\n"; }
};
```

//1. `static_cast` — unchecked(fast, but you must be sure)
void q61() {
	Base61* b = new Derived61();          // upcast: implicit & safe

	Derived61* d = static_cast<Derived61*>(b);  // downcast: NO runtime check
	d->derivedOnly();                 // OK because *b really is a Derived
	delete b;
	//⚠️ If `b` actually pointed to a plain `Base`, `static_cast` would still "succeed" 
	// at compile time, and calling `derivedOnly()` would be undefined behavior.

	Base61* b = new Base61();
	Derived61* d = static_cast<Derived61*>(b);  // compiles fine...
	d->derivedOnly();                        // silent disaster
}

// 2. `dynamic_cast` (pointer)— checked, returns `nullptr` on failure
//Requires the base class to be polymorphic(have a virtual function).

void q62() {
	Base61* b1 = new Derived61();
	Base61* b2 = new Base61();
	if (Derived61* d = dynamic_cast<Derived61*>(b1))
		d->derivedOnly();            // runs: b1 really is a Derived
	if (Derived61* d = dynamic_cast<Derived61*>(b2))
		d->derivedOnly();            // never runs
	else
		std::cout << "b2 is not a Derived\n";  // printed
	delete b1;
	delete b2;
}

// 3. `dynamic_cast` (reference)— throws `std::bad_cast` on failure
void q62() {
	Base61* b2 = new Base61();
	try {
		Base61& ref = *b2;// b2 is really a Base
		Derived61& d = dynamic_cast<Derived61&>(ref);
		d.derivedOnly();
	}
	catch (const std::bad_cast& e) {
		std::cout << "bad_cast: " << e.what() << "\n";
	}
}

// 4. C - style cast — avoid
Base61* b = new Base61();
Derived61* d = (Derived61*)b;   // behaves like static_cast here, but hides intent


Prefer the named casts — they document what you mean

//Rule of thumb: use `dynamic_cast` when you're not 100% certain of the dynamic type; 
// use `static_cast` only when you've already verified the type(e.g., you just created the 
// object, or you checked a type tag yourself).
// And if you find yourself downcasting a lot, consider whether a virtual function or the 
// visitor pattern would be cleaner