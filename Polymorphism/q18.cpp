//18. What is a memory leak in programming ?
// Memory leaks in C++

//A** memory leak** happens when a program allocates memory on the heap but loses every pointer to it without freeing it.The block stays reserved until the process ends, even though nothing uses it anymore.In long - running programs(servers, daemons) leaks accumulate and eventually exhaust memory and crash the process.

// How a leak happens — example

#include <iostream>
void leak() {
    int* p = new int[1000];   // allocate
    // ... use p ...
    // forgot: delete[] p;
}                              // p goes out of scope → pointer lost,
                               // but the heap block is still allocated
int main() {
    for (int i = 0; i < 1'000'000; ++i)
        leak();                // each call loses ~4 KB → memory keeps growing
}

//When `leak()` returns, `p` is destroyed, but the block it pointed to remains — unreachable and unfreeable.That's the leak.

//Common causes

//1. Forgotten `delete` / `delete[]`


void f() {
    int* p = new int(42);
    // no delete p → leak
}

// 2. Wrong `delete` form for arrays


void f() {
    int* arr = new int[10];
    delete arr;      // wrong: needs delete[] → undefined behavior, usually leak/corruption
}

// 3. Losing the pointer on reassignment

void f() {
    int* p = new int(1);
    p = new int(2);  // the first block (value 1) is lost → leak
    delete p;        // only the second is freed
}

//4. Early `return` or an exception before `delete`


void f(bool fail) {
    int* p = new int[100];
    if (fail)
        return;          // leak: delete[] p never runs
    delete[] p;
}
//Exceptions are even more dangerous :

void g() {
    Resource* r = new Resource();
    mightThrow();        // if this throws, delete r never runs → leak
    delete r;
}

// 5. A class without a proper destructor / Rule of Three

class Buffer {
    int* data;
public:
    Buffer(size_t n) : data(new int[n]) {}
    ~Buffer() { delete[] data; }   // OK
};

class BadBuffer {
    int* data;
public:
    BadBuffer(size_t n) : data(new int[n]) {}
    // no destructor → memory leaks when the object is destroyed
};

// 6. Cyclic `std::shared_ptr` references

struct Node {
    std::shared_ptr<Node> next;
};
auto a = std::make_shared<Node>();
auto b = std::make_shared<Node>();
a->next = b;
b->next = a;      // cycle: ref counts never reach 0 → leak
// even after a and b go out of scope, the objects stay alive


// How to avoid leaks

// 1. RAII — resources freed by destructors

#include <memory>
void safe() {
    auto p = std::make_unique<int[]>(1000);  // freed automatically
}                                             // even if an exception is thrown
// 2. Smart pointers instead of raw `new`/`delete`

#include <memory>
void f() {
    auto obj = std::make_unique<Resource>();   // unique ownership
    auto shared = std::make_shared<Resource>();   // shared ownership
}   // both freed automatically

// 3. STL containers instead of hand - rolled arrays

#include <vector>

void f() {
    std::vector<int> v(1000);   // memory managed automatically
}                               // no leak possible

// 4. Break `shared_ptr` cycles with `weak_ptr`

struct Node {
    std::weak_ptr<Node> next;   // weak ref doesn't bump the count → no cycle
};

// 5. Rule of Zero / Rule of Three - Five for your own classes

//If a class owns a resource, define destructor, copy, and move — 
// or (better)hold smart pointers / containers inside so you don't write them manually.

// 6. Tools to find leaks

//In one line : a memory leak is an allocated block that's never freed because its pointer was lost;
// in C++ you prevent it with RAII and smart pointers, and detect it with Valgrind/AddressSanitizer.