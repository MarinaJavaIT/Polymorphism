//19. Can you give an example of a specific class hierarchy(at least 4 classes) containing at least one abstract class ?
// A class hierarchy with an abstract class (4 + classes)

//Here's a concrete example: a **shape** hierarchy. `Shape` is **abstract** (pure virtual `area()` and `name()`), and three concrete classes derive from it — with one of them (`Rectangle`) having its own derived class to make the hierarchy deeper.

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <memory>

// ---------- Abstract base ----------
class Shape {
public:
    virtual ~Shape() = default;

    // Pure virtual → Shape is abstract, cannot be instantiated
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;

    // Ordinary virtual with a default implementation (overridable)
    virtual void describe() const {
        std::cout << name() << ": area=" << area()
            << " perimeter=" << perimeter() << "\n";
    }
};

// ---------- Concrete class 1 ----------
class Circle : public Shape {
    double r;
public:
    explicit Circle(double r) : r(r) {}
    double area() const override { return M_PI * r * r; }
    double perimeter() const override { return 2 * M_PI * r; }
    std::string name() const override { return "Circle"; }
};

// ---------- Concrete class 2 ----------
class Rectangle : public Shape {
protected:
    double w, h;
public:
    Rectangle(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
    double perimeter() const override { return 2 * (w + h); }
    std::string name() const override { return "Rectangle"; }
};

// ---------- Class 3: derives from Rectangle (deeper hierarchy) ----------
class Square : public Rectangle {
public:
    explicit Square(double side) : Rectangle(side, side) {}
    std::string name() const override { return "Square"; }
    // area/perimeter inherited from Rectangle — correct as-is
};

// ---------- Class 4: triangle ----------
class Triangle : public Shape {
    double a, b, c;
public:
    Triangle(double a, double b, double c) : a(a), b(b), c(c) {}
    double area() const override {                 // Heron's formula
        double s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }
    double perimeter() const override { return a + b + c; }
    std::string name() const override { return "Triangle"; }
};

// ---------- Client code works only against Shape ----------
int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rectangle>(3, 4));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));

    for (const auto& s : shapes)
        s->describe();        // dynamic dispatch

    // Shape s;               // ERROR: abstract class
}

//Output:
//
//Circle : area = 12.5664 perimeter = 12.5664
//Rectangle : area = 12 perimeter = 14
//Square : area = 25 perimeter = 20
//Triangle : area = 6 perimeter = 12


// Hierarchy in diagram form

 //         Shape          ← abstract(pure virtual area / perimeter / name)
 //        /  |  \
//Circle  Rectangle  Triangle
////|
///Square

//
//
//Points illustrated
//- **Abstract class :**`Shape` cannot be instantiated — you can only have pointers / references to it(`Shape*`, `Shape&`).
//    - **Pure virtual contract:**every concrete class must implement `area()`, `perimeter()`, and `name()`.
//- **Ordinary virtual with default:**`describe()` is virtual but has a body; derived classes may inherit it(`Circle`, `Square`, `Triangle` all use it as - is).
//    - **Deeper hierarchy : **`Square : Rectangle` reuses `Rectangle`'s implementation and only overrides `name()` — showing that not every class needs to redefine everything.
//    - **Polymorphism :**the `std: : vector<std::unique_ptr<Shape>>` holds heterogeneous types; `s- > describe()` calls the right version for each — the* object's* type decides (dynamic dispatch).
//    - **RAII:**`unique_ptr` frees every object automatically — no manual `delete`, no leaks.
//
//    This is the canonical shape / abstract - interface pattern : a pure - virtual base defines the contract, concrete classes supply implementations, and client code depends only on the abstract interface.