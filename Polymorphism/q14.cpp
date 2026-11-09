//14. Does using virtual functions allow the base class to define a generalized interface ?
// Do virtual functions let a base class define a generalized interface ?

//Yes — that's one of the primary purposes of virtual functions. 
// The base class declares a generalized interface (the operations every derived type must support), 
// and each derived class supplies its own implementation. 
// Client code then works against the base interface without knowing the concrete types.

//This is the essence of runtime polymorphism and the open/closed principle:
// you can add new derived types without changing code that uses the base interface.

// Example : a generalized `Shape141` interface

#include <iostream>
#include <vector>
#include <memory>

class Shape141 {
public:
	virtual ~Shape141() = default;

	// Generalized interface: every Shape141 must provide these
	virtual double area() const = 0;
	virtual double perimeter() const = 0;
	virtual const char* name() const = 0;
};

class Circle141 : public Shape141 {
	double r;
public:
	explicit Circle141(double r) : r(r) {}
	double area() const override { return 3.14159 * r * r; }
	double perimeter() const override { return 2 * 3.14159 * r; }
	const char* name() const override { return "Circle141"; }
};

class Rectangle141 : public Shape141 {
	double w, h;
public:
	Rectangle141(double w, double h) : w(w), h(h) {}
	double area() const override { return w * h; }
	double perimeter() const override { return 2 * (w + h); }
	const char* name() const override { return "Rectangle141"; }
};

class Triangle141 : public Shape141 {
	double a, b, c;
public:
	Triangle141(double a, double b, double c) : a(a), b(b), c(c) {}
	double area() const override {                 // Heron's formula
		double s = (a + b + c) / 2;
		return std::sqrt(s * (s - a) * (s - b) * (s - c));
	}
	double perimeter() const override { return a + b + c; }
	const char* name() const override { return "Triangle141"; }
};

// Client code works only against the interface

void describe(const Shape141& s) {                   // knows nothing concrete
	std::cout << s.name() << ": area=" << s.area()
		<< " perimeter=" << s.perimeter() << "\n";
}

void q141() {
	std::vector<std::unique_ptr<Shape141>> Shape141s;
	Shape141s.push_back(std::make_unique<Circle141>(2));
	Shape141s.push_back(std::make_unique<Rectangle141>(3, 4));
	Shape141s.push_back(std::make_unique<Triangle141>(3, 4, 5));

	for (const auto& s : Shape141s)
		describe(*s);// dynamic dispatch picks each type's methods
}

//Output:
//Circle141 : area = 12.5664 perimeter = 12.5664
//Rectangle141 : area = 12 perimeter = 14
//Triangle141 : area = 6 perimeter = 12

//`describe` and the loop never mention `Circle141`, `Rectangle141`, or `Triangle141`. 
// They program to the `Shape141` interface. 
// Adding a new Shape141 requires no change to this code — just a new derived class.

// Why this is powerful?
//Uniform treatment : a container of `Shape141*` holds heterogeneous objects; one loop handles them all.
//Extensibility: new types plug into the existing interface with zero edits to client code.
//Encapsulation : each type owns its own algorithm; callers don't switch on type.
//Abstraction: `Shape141` is abstract(`= 0`), so it only exists as a contract — you can't instantiate a meaningless "Shape141."

// The interface is a contract
//Because `area()`, `perimeter()`, and `name()` are pure virtual, 
// every concrete derived class must implement them — 
// the base enforces the contract at compile time.
// Forget one and the class stays abstract(won't compile when instantiated). 
// That's the generalized interface being genuinely enforced, not just documented.

// Contrast: without virtual functions
//Without virtual dispatch you'd need type checks everywhere — brittle and not extensible:

// Bad: client must know every concrete type
double areaOf(Shape141 * s) {
	if (auto* c = dynamic_cast<Circle141*>(s))    return 3.14 * c->r * c->r;
	if (auto* r = dynamic_cast<Rectangle141*>(s)) return r->w * r->h;
	// ... add a case for every new type, and edit every such function
}

//Virtual functions replace this entire "switch on type" pattern with one indirect call.

//Bottom line: yes — virtual functions(especially pure virtual ones) are exactly
// how a base class defines a generalized, polymorphic interface.
// The base specifies* what* operations exist; derived classes decide* how* they work; 
// client code depends only on the abstract interface.