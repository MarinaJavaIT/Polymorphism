//13. Should the prototypes of a virtual function and its redefinitions be exactly the same ?
// Must a virtual function and its overrides have identical prototypes ?

//For a true override (so that dynamic dispatch calls your Derived131 version), 
// the prototype must match the Base131 virtual function exactly 
// — same name, same parameter types, same `const`/`volatile` qualification, 
// and same ref - qualifiers.The return type must also match, except for one special case: 
// a covariant return type(a pointer/reference to a Derived131 class).
// If the signature differs, you don't override — you hide the Base131 function, 
// which is usually a silent bug.
//Exact match — a real override

#include <iostream>

class Base131131 {
public:
	virtual ~Base131() = default;
	virtual void f(int x) const { std::cout << "Base131::f(" << x << ")\n"; }
};

class Derived131131 : public Base131 {
public:
	void f(int x) const override { std::cout << "Derived131::f(" << x << ")\n"; }
};

void q131() {
	Base131* p = new Derived131();
	p->f(42);// Derived131::f(42)  ← override works
	delete p;
}

//The `override` keyword makes the compiler verify the match.
// If it doesn't match, you get a compile error instead of a silent bug.

// What breaks the match?
//Any of these differences means it's no longer an override:

class Base132 {
public:
	virtual void f(int x) const;
};

class Bad132 : public Base132 {
public:
	void f(int x) override;              // ERROR: missing const → not an override
	// void f(double x) override;        // ERROR: different param type
	// int  f(int x) const override;     // ERROR: different return type (non-covariant)
	// void f(int x, int y) override;    // ERROR: different parameter list
};
//Without `override`, these would compile silently but would hide `Base132::f` 
// rather than override it — and calls through a `Base132*` would still run `Base132::f`. 
// That's the classic trap `override` is designed to catch.

// Hiding without `override` — the silent bug

class Base133 {
public:
	virtual void f(int x) const { std::cout << "Base133::f\n"; }
};

class Derived133 : public Base133 {
public:
	void f(int x) { std::cout << "Derived133::f\n"; }   // forgot const!
};

Base133* p = new Derived133();
p->f(1);        // "Base133::f" — not overridden, just hidden. Surprising!
delete p;
//Here `Derived133::f` is a different function(non - const).
// It hides `Base133::f` for direct `Derived133` calls, but through `Base133*` the virtual 
// dispatch still finds `Base133::f`.

//The exception : covariant return types

//The return type may differ only if it is a pointer or reference to a class Derived
// from the Base function's return type. This is called a covariant return type.

class Base134 {
public:
	virtual Base134* clone() const { return new Base134(*this); }
};

class Derived134 : public Base134 {
public:
	Derived134* clone() const override {   // OK: Derived134* is covariant with Base134*
		return new Derived134(*this);
	}
};
//This is the only allowed difference in the prototype.
// Everything else — name, parameters, cv - qualifiers, ref - qualifiers — must match exactly.

// Note on default arguments
//Default arguments are not part of the function type, so they don't affect overriding. 
// But beware: the default value used is the one from the static type of the pointer, 
// while the function called is from the dynamic type — a well-known source of confusion:

class Base135 {
public:
	virtual void f(int x = 10) { std::cout << "Base135 " << x << "\n"; }
};
class Derived135 : public Base135 {
public:
	void f(int x = 20) override { std::cout << "Derived135 " << x << "\n"; }
};

Base135* p = new Derived135();
p->f();     // "Derived135 10" — Derived135::f runs, but Base135's default (10) is used

//So the* prototype* must match, but default argument values can differ — 
// and you generally shouldn't rely on it

//Bottom line: yes — a virtual function and its redefinitions 
// must have the same prototype(name, parameters, cv - and ref - qualifiers, return type), 
// with the single exception of covariant return types.Always mark overrides with `override` 
// so the compiler enforces the exact match and turns a silent "hide instead of override" bug 
// into a compile error.