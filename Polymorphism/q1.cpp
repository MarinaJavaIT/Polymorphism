//1. What defines the concept of "polymorphism" in programming ? example C++
//Polymorphism in Programming





//  контекст
// 
// 
// 
//** Polymorphism** (from Greek : "poly" = many, "morph" = forms) 
// is the ability of a single interface, function, or object to behave differently depending on the context — 
// typically the actual type of the object being operated on.It allows you to write code that works on a general type while the 
// specific behavior is determined at runtime(or compile time).
//
//In C++, polymorphism comes in two main flavors :
//1.Compile - time(static) polymorphism — resolved by the compiler :
//-1 Function overloading
//-2 Operator overloading
//-3 Templates шаблоны template
//2.Runtime(dynamic) polymorphism — resolved at runtime via 
// virtual functions and inheritance.

// Example 1: Runtime Polymorphism(Virtual Functions)
#include <iostream>
#include <vector>
#include <memory>
class Shape {
    int area;
public:
    virtual void draw() const {           // virtual → dynamic dispatch
        std::cout << "Drawing a generic shape\n";
    }
    virtual double area() const = 0;      // pure virtual → abstract class
    virtual ~Shape() {
        delete arr;
    };           // virtual destructor (important!)
    Shape(int val, int i) {
        arr = new int[val];
    };           // virtual destructor (important!)
    Shape(int i) {
        this.i = i;
        arr = new int[10];// heap
    };


};// base
class Circle : public Shape {
    double radius;
public:
    explicit Circle(double radius) : radius(radius) {}
    void draw() const override {
        std::cout << "Drawing a Circle\n";
    }
    double area() const override { return 3.14159 * radius * radius; }
};
class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double width, double height) : width(width), height(height) {}
    void draw() const override {
        std::cout << "Drawing a Rectangle\n";
    }
    double area() const override { return width * height; }
};
class RectangleNew {};
int q1_0() {
    std::vector<std::unique_ptr<Shape>> shapes;// <>
    //Shape shapes1[];// <>
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));


    for(int i=0;i<10;i++)
    for (const auto& s : shapes) {// each [0] [1] [2]....
        s->draw();                                     // same call, different behavior
        std::cout << "  area = " << s->area() << "\n";
    }
}

//** Output:**
//Drawing a Circle
//area = 12.5664
//Drawing a Rectangle
//area = 12

// Example 2 : Compile - time Polymorphism(Function Overloading)
#include <iostream>

void print(int x) { std::cout << "int: " << x << "\n"; }
void print(double x) { std::cout << "double: " << x << "\n"; }
void print(const std::string & s) { std::cout << "string: " << s << "\n"; }

int q1_1() {
    print(42);           // int version
    print(3.14);         // double version
    print("hello");      // string version
}

//The compiler picks the correct overload based on argument types — no runtime cost.

// Example 3: Compile - time Polymorphism(Templates)
#include <iostream>

template <typename T>
T add(T a, T b) { return a + b; }

int q1_2() {
    std::cout << add(2, 3) << "\n";             // int
    std::cout << add(2.5, 3.1) << "\n";         // double
    std::cout << add<std::string>("a", "b") << "\n"; // std::string
}

//One template generates a distinct function for each type it's instantiated with — again "many forms" from a single definition.

//Why it matters : Polymorphism lets you extend behavior without modifying existing code(open/closed principle).
// You can add a new `Triangle : Shape` class and the `for` loop above works unchanged — it doesn't need to know about `Triangle` at all