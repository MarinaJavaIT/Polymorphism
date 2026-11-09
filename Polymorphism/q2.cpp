//2. What constructions of the C++ language relate to the polymorphism of the compilation stage ? example C++
//Compile - Time(Static) Polymorphism in C++
//Compile - time polymorphism means the decision about which function or code to execute is made by the compiler 
// before the program runs.It is resolved based on types known at compile time.C++ provides several language constructs for this.

//The main constructions are :

//1. Function overloading 
//2. Operator overloading 
//3. Templates (function templates and class templates)
//4. Template specialization (a form of the above)
//5. CRTP (Curiously Recurring Template Pattern) — a design idiom built on templates
//6. `constexpr` / `if constexpr` — compile - time branching(modern C++)

// 1. Function Overloading
//Multiple functions with the same name but different parameter lists.The compiler picks the right one based on the argument types.

#include <iostream>
void print(int x) { std::cout << "int: " << x << "\n"; }
void print(double x) { std::cout << "double: " << x << "\n"; }
void print(const std::string & s) { std::cout << "string: " << s << "\n"; }

int q2_0() {
    print(42);        // int overload
    print(3.14);      // double overload
    print("hello");   // string overload
}

//The compiler resolves the call statically — there is no runtime dispatch.

// 2. Operator Overloading

//Same operator symbol, different meaning depending on operand types.
#include <iostream>
struct Vec2 {
    double x, y;
};
Vec2 operator+(const Vec2 & a, const Vec2 & b) {
    return { a.x + b.x, a.y + b.y };
}
std::ostream& operator<<(std::ostream & os, const Vec2 & v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int q2_1() {
    Vec2 a{ 1, 2 }, b{ 3, 4 };
    std::cout << a + b << "\n";   // (4, 6)
}
//`+` here means "vector addition" — chosen at compile time by overload resolution.

// 3. Function Templates

//A single function definition that the compiler instantiates for each type it is used with.

#include <iostream>
template <typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}
int q2_2() {
    std::cout << maxOf(3, 7) << "\n";  // T = int
    std::cout << maxOf(2.5, 1.1) << "\n";  // T = double
    std::cout << maxOf(std::string("a"), std::string("b")) << "\n";
}

//The compiler generates a separate specialization of `maxOf` for each type — all resolved at compile time.

// 4. Class Templates
//Same idea for types. `std::vector<int>` and `std::vector<double>` are distinct types generated from one template.

#include <iostream>
template <typename T>
class Box {
    T value;
public:
    explicit Box(T v) : value(v) {}
    T get() const { return value; }
};

int q2_3() {
    Box<int>    bi(42);
    Box<double> bd(3.14);
    std::cout << bi.get() << " " << bd.get() << "\n";
}


//5. Template Specialization
//You can provide a specialized implementation for a particular type — still chosen at compile time.
#include <iostream>

template <typename T>
struct Printer {
    static void print(const T& v) { std::cout << "generic: " << v << "\n"; }
};

template <>
struct Printer<bool> {                          // full specialization
    static void print(bool v) {
        std::cout << "bool: " << (v ? "true" : "false") << "\n";
    }
};

int q2_4() {
    Printer<int>::print(5);       // generic
    Printer<bool>::print(true);   // specialized
}
// 6. CRTP — Curiously Recurring Template Pattern
//A class passes itself as a template argument to its base — giving "static polymorphism" without virtual functions.

#include <iostream>
template <typename Derived>
class Shape {
public:
    void draw() const {
        static_cast<const Derived*>(this)->drawImpl();   // resolved at compile time
    }
};

class Circle : public Shape<Circle> {
public:
    void drawImpl() const { std::cout << "Circle\n"; }
};

class Square : public Shape<Square> {
public:
    void drawImpl() const { std::cout << "Square\n"; }
};

template <typename T>
void render(const Shape<T>&s) { s.draw(); }   // no vtable needed

int q2_5() {
    render(Circle{});
    render(Square{});
}
//The call `s.draw()` is fully resolved at compile time — no virtual dispatch, no vtable.This is the classic "static polymorphism" idiom.

// 7. Compile - Time Branching : `if constexpr` (C++17)
//Chooses a branch at compile time based on a template parameter or `constexpr` condition.
#include <iostream>
#include <type_traits>

template <typename T>
void describe(const T & v) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integral: " << v << "\n";
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "floating: " << v << "\n";
    }
    else {
        std::cout << "other\n";
    }
}

int q2_5() {
    describe(42);      // integral branch compiled
    describe(3.14);    // floating branch compiled
}
//Only the matching branch is compiled — the others are discarded entirely.

//Common thread : in every case, the compiler knows the exact types involved and emits the correct call directly.
// There is no vtable lookup and no runtime type check — which is why compile - time polymorphism is generally faster than 
// runtime polymorphism(at the cost of larger binaries and less flexibility to add types at runtime).