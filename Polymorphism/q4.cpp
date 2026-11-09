//4. Can a pointer to a base class refer to any object of a child class ? 
// Short answe: Yes — with conditions. A pointer(or reference) to a base class can
// point to an object of any derived class**, provided:

//1.The derived class inherits the base(usually `public`).
//2.The base is accessible (not `private`/`protected` inheritance).
//3.To get the child's behavior through that pointer, the methods must be `virtual` (otherwise the base version is called).
//4.If you delete through the base pointer, the base destructor must be virtual.

// ✅ Example 1: The basic case — works

#include <iostream>
class Base1 {
public:
	virtual void hello() const { std::cout << "Base\n"; }
	virtual ~Base1() = default;
};
class Child1 : public Base1 {
public:
	void hello() const override { std::cout << "Child\n"; }
};
int q4_0() {
	Child1 c;
	Base1* p = &c;      // ✅ base pointer → Child object
	p->hello();        // "Child" — virtual dispatch
	Base1& r = c;       // ✅ base reference → Child object
	r.hello();         // "Child"
}
//`p` has type `Base*` but points to a `Child`. Through it you see only the base interface, 
// but virtual calls reach `Child`.

// ✅ Example 2: One base pointer, many child types

#include <iostream>
#include <memory>
#include <vector>

class Animal {
public:
	virtual void speak() const = 0;          // pure virtual → abstract
	virtual ~Animal() = default;
};

class Dog : public Animal {
public:
	void speak() const override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
	void speak() const override { std::cout << "Meow!\n"; }
};

class Cow : public Animal {
public:
	void speak() const override { std::cout << "Moo!\n"; }
};

int q4_1() {
	std::vector<std::unique_ptr<Animal>> zoo;
	zoo.push_back(std::make_unique<Dog>());
	zoo.push_back(std::make_unique<Cat>());
	zoo.push_back(std::make_unique<Cow>());

	for (const auto& a : zoo)
		a->speak();     // each base pointer → a different child object
}


//** Output:**
//Woof!
//Meow!
//Moo!

//One `Animal*` type refers to any of the child objects — this is the whole point of runtime polymorphism.

// ⚠️ Example 3: `private` inheritance breaks it

class Base3 {
public: virtual ~Base3() = default;
};

class ChildPriv : private Base3 {};   // ❌ private inheritance

int q4_2() {
	ChildPriv c;
	// Base* p = &c;   // ❌ compile error: Base is an inaccessible base
}
//From the outside, `ChildPriv` is not a `Base` — the implicit conversion is forbidden.

// ⚠️ Example 4: Without `virtual`, the base version is called
#include <iostream>
class Base4 {
public:
	void hello() const { std::cout << "Base\n"; }   // NOT virtual
};
class Child4 : public Base4 {
public:
	void hello() const { std::cout << "Child\n"; }  // hides, doesn't override
};
int q4_3() {
	Child4 c;
	Base4* p = &c;
	p->hello();   // "Base" — static binding by the pointer's type!
}
//This is the classic trap : without `virtual`, the compiler calls `Base::hello` because it 
// only knows the pointer's static type.

// ⚠️ Example 5: Downcast requires `dynamic_cast` + check
//Upcast(child → base) is implicit and safe.The reverse needs an explicit, checked cast.

#include <iostream>
class Base5 {
public: virtual ~Base5() = default;
};
class Child5 : public Base5 { public: void onlyChild() { std::cout << "child only\n"; } };

int q4_5() {
	Base5* p1 = new Child5();
	Base5* p2 = new Base5();

	if (auto* c = dynamic_cast<Child5*>(p1))   // ✅ succeeds
		c->onlyChild();

	if (auto* c = dynamic_cast<Child5*>(p2))   // ❌ returns nullptr
		c->onlyChild();
	else
		std::cout << "p2 is not a Child5\n";
	delete p1;
	delete p2;
}
//`dynamic_cast` returns `nullptr` if the object isn't actually of that type — safe runtime check.

// ⚠️ Example 6: Multiple inheritance — different subobject addresses
//With multiple inheritance, the same object can have different addresses depending on which 
// base pointer you view it through.The compiler adjusts the pointer on upcast.
#include <iostream>

class A {
public: int a = 1;
};
class B {
public: int b = 2;
};
class C : public A, public B { public: int c = 3; };

int q4_6() {
	C obj;
	A* pa = &obj;    // ✅ points to the A-subobject
	B* pb = &obj;    // ✅ points to the B-subobject (address may differ!)

	std::cout << (void*)pa << "\n";
	std::cout << (void*)pb << "\n";   // often a different address
	std::cout << (void*)&obj << "\n";
}
//Both pointers are valid and refer to the same `C` object — just different subobjects.

// ⚠️ Example 7: Virtual destructor is mandatory for polymorphic deletion
#include <iostream>
class Base7 {
public:
	virtual ~Base7() = default;   // ✅ essential
};

class Child7 : public Base7 {
	int* data = new int[100];
public:
	~Child7() override { delete[] data; std::cout << "~Child7\n"; }
};

int q4_7() {
	Base7* p = new Child7();
	delete p;   // ~Child7 then ~Base7 — correct only because ~Base is virtual
}
//Without `virtual ~Base()`, deleting through `Base*` is undefined behavior(the `Child` part 
// is never destroyed).

// ⚠️ Example 8: Object slicing — a trap with values, not pointers
//A base pointer is fine.But passing a child by value into a base parameter slices off the
// child part.

class Base8 { public: virtual void hello() const { std::cout << "Base\n"; } };
class Child8 : public Base8 { public: void hello() const override { std::cout << "Child\n"; } };

void byValue(Base8 b) { b.hello(); }   // ❌ slicing: copies only Base part
void byRef(Base8 & b) { b.hello(); }   // ✅ polymorphic

int q4_8() {
	Child8 c;
	byValue(c);   // "Base" — child part sliced away
	byRef(c);     // "Child"
}
//Pointers and references preserve polymorphism; passing by value does not.

//📌 Summary 🧠 One - sentence takeaway

// A base - class pointer can refer to any object of a derived class (with `public` inheritance), 
// but it "sees" only the base interface — and which implementation runs is decided by `virtual`.