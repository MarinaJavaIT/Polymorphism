// Can You Access Derived-Class-Specific Members Through a Base - Class Pointer ?

//Not directly — but yes, after an explicit, checked downcast (`dynamic_cast` or `static_cast`).
// Through a `Base*` the compiler only knows the base interface, so derived - specific members 
// are invisible.You must first convert the pointer back to the derived type.

// ❌ Why not directly — the compiler only sees the base

#include <iostream>

class Base51 {
public:
	virtual ~Base51() = default;
	void baseOnly() const { std::cout << "base\n"; }
};

class Child51 : public Base51 {
public:
	void childOnly() const { std::cout << "child\n"; }   // NOT in Base
};

int q5_1() {
	Child51 c;
	Base51* p = &c;           // ✅ base pointer → Child object
	p->baseOnly();           // ✅ OK
	// p->childOnly();       // ❌ compile error: Base has no member 'childOnly'
}
//The compiler resolves members by the static type of the pointer(`Base*`), not by the object's 
// actual type. `childOnly` isn't declared in `Base`, so it's simply unknown at that point.

// ✅ Solution 1: `dynamic_cast` — safe downcast with a runtime check

// Use this when you don't know for sure the object's dynamic type. `dynamic_cast` returns 
// `nullptr` (for pointers) or throws `std::bad_cast` (for references) if the object isn't actually
// of the target type.

#include <iostream>
class Base52 {
public:
	virtual ~Base52() = default;   // required for dynamic_cast
};
class Child52 : public Base52 {
public:
	void childOnly() const { std::cout << "child-specific!\n"; }
	int  extraData = 42;
};

int q5_2() {
	Base52* p1 = new Child52();
	Base52* p2 = new Base52();
	if (auto* c = dynamic_cast<Child52*>(p1)) {   // ✅ succeeds
		c->childOnly();                          // "child-specific!"
		std::cout << c->extraData << "\n";       // 42
	}

	if (auto* c = dynamic_cast<Child52*>(p2)) {   // ❌ returns nullptr
		c->childOnly();
	}
	else {
		std::cout << "p2 is not a Child\n";
	}
	delete p1;
	delete p2;
}

//** Output:**
//child - specific!
//42
//p2 is not a Child

//This is the correct, safe way: check the cast result before using it.
// 
// ✅ Solution 2 : `static_cast` — fast, but unchecked
//Use only when you know for certain the object is of the target type(e.g., you just created it).
// No runtime check → undefined behavior if you're wrong.
int q5_3() {
	Base52* p = new Child52();
	auto* c = static_cast<Child52*>(p);   // ✅ works if *p really is a Child
	c->childOnly();                     // fast, no check

	Base52* q = new Base52();
	auto* bad = static_cast<Child52*>(q); // ⚠️ compiles, but WRONG
	// bad->childOnly();                // ❌ undefined behavior
}

//`static_cast` is cheaper than `dynamic_cast` (no RTTI lookup) but puts the responsibility on you.

//✅ Solution 3: virtual function — the "clean" design way

//If the derived - specific behavior is a legitimate part of the abstraction, 
// make it virtual in the base.Then you don't need any cast — you call it 
// through the base pointer and the derived version runs.

#include <iostream>

class Base53 {
public:
	virtual void describe() const = 0;   // part of the base interface
	virtual ~Base() = default;
};

class Child53 : public Base53 {
public:
	void describe() const override {
		std::cout << "Child, extraData = " << extraData << "\n";
	}
	int extraData = 42;                  // used internally, no cast needed
};

int q53() {
	Base53* p = new Child53();
	p->describe();   // ✅ "Child, extraData = 42" — no downcast required
	delete p;
}

// Rule of thumb: if you find yourself casting a lot to reach derived members, 
// the design probably wants a virtual function instead.

//⚠️ Solution 4 : `dynamic_cast` on references — throws instead of `nullptr`
void use(Base & b) {
	try {
		Child& c = dynamic_cast<Child&>(b);   // throws if not a Child
		c.childOnly();
	}
	catch (const std::bad_cast&) {
		std::cout << "not a Child\n";
	}
}
//Same idea, but the failure mode is an exception rather than `nullptr`.

//🧠 One - sentence takeaway

//Through a `Base*` you can only see the base interface; to reach derived - 
// specific members you must downcast — safely with `dynamic_cast` (checked) or 
// blindly with `static_cast` (fast but risky) — and the cleanest solution is usually 
// to make the behavior `virtual` in the base instead.