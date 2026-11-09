//17. What problems are possible with multiple inheritance ?
//Problems with multiple inheritance in C++

//Multiple inheritance is powerful but brings several well - known pitfalls.Here are the main ones, each with an example.

// 1. Ambiguity of inherited names

//If two bases provide a member with the same name, an unqualified use is** ambiguous** — a compile error.

#include <iostream>

class A {
public: void hello() const { std::cout << "A\n"; }
};
class B {
public: void hello() const { std::cout << "B\n"; }
};

class C : public A, public B {};

void q171() {
	C c;
	// c.hello();          // ERROR: ambiguous — A::hello or B::hello?
	c.A::hello();          // "A" — must qualify
	c.B::hello();          // "B"
}

//Even identical - looking members from unrelated bases don't merge — you must disambiguate every use. This gets noisy in large hierarchies.

// 2. The diamond problem(duplicated base)

//If two bases share a common ancestor, the derived object contains** two copies** of that ancestor.Members of the common base become ambiguous, and state is duplicated.

class Person {
public: int id = 0;
};
class Student : public Person {};
class Employee : public Person {};

class TeachingAssistant : public Student, public Employee {};
void q172() {
	TeachingAssistant ta;
	// ta.id = 5;                 // ERROR: ambiguous — two Person subobjects
	ta.Student::id = 1;          // one copy
	ta.Employee::id = 2;          // a *different* copy
	// ta has sizeof ≥ 2 * sizeof(Person)
}
//`TeachingAssistant` has** two** `Person` subobjects.If `Person` had a constructor with side effects, it'd run twice. If it had a resource, that resource would be duplicated and possibly freed twice.

//Fix: virtual inheritance

class Student : virtual public Person {};
class Employee : virtual public Person {};
class TeachingAssistant : public Student, public Employee {};
// now exactly ONE shared Person subobject
void q173() {
	TeachingAssistant ta;
	ta.id = 5;      // OK — unambiguous
}
//But virtual inheritance adds overhead(extra indirection via a virtual base pointer) and complicates construction — the** most - derived class * *becomes responsible for initializing the virtual base.

class Person {
public:
	Person(int id) : id(id) {}
	int id;
};
class Student : virtual public Person { public: Student(int id) : Person(id) {} };
class Employee : virtual public Person { public: Employee(int id) : Person(id) {} };
class TA : public Student, public Employee {
public:
	// Must initialize the virtual base directly — Student/Employee's
	// initializations of Person are IGNORED here:
	TA(int id) : Person(id), Student(id), Employee(id) {}
};

//Forgetting this rule causes subtle bugs(the virtual base default - constructed instead of using the intended value).

// 3. Object layout and `sizeof` bloat

//Each non - virtual base contributes a subobject.Multiple bases → larger objects, more padding, more complex pointer adjustments.

class A {
	int a;
};
class B {
	int b;
};
class C : public A, public B {
	int c;
};

// sizeof(C) >= sizeof(A) + sizeof(B) + sizeof(int), plus padding/alignment

// 4. Pointer adjustment / address differences
//A pointer to a derived object and a pointer to one of its base subobjects may have 
// different addresses.
// Converting between them adjusts the pointer — a source of confusion, especially with 
// `void*` or casts.

C c;
A* pa = &c;
B* pb = &c;
// pa and pb may hold DIFFERENT numeric addresses, both "pointing at c"!

void* pv = &c;            // points to the C subobject
void* pv2 = static_cast<B*>(&c);  // may differ from pv — surprising

//Comparing or casting through `void*` can silently break because of these offsets.

// 5. Ambiguous conversions / overload resolution

//A derived object may convert to several bases, making a function overload or cast ambiguous.

void f(A*);
void f(B*);
C c;
// f(&c);        // ERROR: ambiguous — converts to both A* and B*

void g(A&);
void g(B&);
// g(c);         // ERROR: ambiguous

//You must cast explicitly : `f(static_cast<A*>(&c));`.

// 6. `dynamic_cast` and cross - casts get complicated

//Casting from one base to a sibling base(a "cross-cast") requires `dynamic_cast` and RTTI, and only works if the base is polymorphic.Virtual inheritance adds indirection to this.

class A {
public: virtual ~A() = default;
};
class B {
public: virtual ~B() = default;
};
class C : public A, public B {};

B b;
// C* c = dynamic_cast<C*>(&b);    // needs the object to really be a C
A* a = new C();
B* bp = dynamic_cast<B*>(a);       // cross-cast: A* → B* through the real object

//These casts are slower and easier to get wrong than single - inheritance up / down casts.

// 7. Non - virtual destructor hazards across multiple bases

//Deleting through a base pointer requires a * *virtual destructor**, and with several bases you must be careful* which* base pointer you delete through.


class A {
public: ~A() {}
};        // non-virtual!
class B {
public: virtual ~B() = default;
};
class C : public A, public B {};

A* pa = new C();
// delete pa;      // UB: ~C and ~B never run — only ~A
B* pb = new C();
delete pb;         // OK: virtual → full destruction

// 8. Increased coupling and design complexity

//Multiple inheritance of implementation tightly couples a class to several bases' internals, 
// complicating maintenance, testing, and reasoning about construction/destruction order 
// (bases constructed left-to-right, destroyed right-to-left).

// The practical guideline

//Use multiple inheritance mainly for** inheriting interfaces** (classes of pure virtual functions) plus** at most one** concrete implementation base.This avoids most of the above :

class Serializable {
public: virtual ~Serializable() = default;
	  virtual std::string serialize() const = 0;
};
class Comparable {
public: virtual ~Comparable() = default;
	  virtual bool lessThan(const Comparable&) const = 0;
};

class Record : public Serializable, public Comparable {  // interfaces only
public:
	std::string serialize() const override { return "record"; }
	bool lessThan(const Comparable&) const override { return false; }
};


//No shared state, no diamond, no ambiguity — just a class implementing several contracts.

// Bottom line : **multiple inheritance can cause name ambiguity, duplicated base subobjects(diamond), pointer - offset surprises, ambiguous conversions, virtual - base construction complexity, and deletion hazards.It's safe and idiomatic when limited to interface inheritance; it becomes fragile when used to combine multiple stateful implementations.