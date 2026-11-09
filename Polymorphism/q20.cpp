//20. What are static, dynamic, and automatic objects ?
// Static, dynamic, and automatic objects in C++

//These terms describe** where and for how long an object lives** — its storage duration and where its memory comes from.Every C++ object has exactly one of these storage durations(plus thread - local, a rarer fourth).

//1. Automatic objects — on the stack

//Allocated automatically when control enters a block, destroyed automatically when it leaves.Lives on the** stack** .This is the default for local variables.

#include <iostream>

void f() {
	int x = 10;             // automatic: created on entry, destroyed on exit
	std::string s = "hi";   // automatic

	if (x > 5) {
		int y = 20;         // automatic, scoped to this block
	}                       // y destroyed here

}                           // x and s destroyed here (s frees its heap buffer!)

//Key traits :
//- Lifetime = the enclosing block(`{}`).
//- Created / destroyed * *every time * *the block runs.
//- Destructor runs automatically at scope exit(including on `return` or exception) — this is the basis of * *RAII * *.
//- Cheap: just a stack - pointer adjustment.

// 2. Static objects — for the whole program

//Allocated** once** and live for the entire program run.There are three flavors :

// a) Local `static` — created once, lives until program end

void counter() {
	static int calls = 0;   // initialized ONCE, persists across calls
	++calls;
	std::cout << "called " << calls << " times\n";
}

int main() {
	counter();   // called 1 times
	counter();   // called 2 times
	counter();   // called 3 times
}

//The variable is initialized the first time control reaches its declaration(thread - safe since C++11) and destroyed at program exit.

// b) Global / namespace - scope objects

int globalCounter = 0;          // static storage duration, whole program
class Logger {
public:
	Logger() { std::cout << "Logger up\n"; }
	~Logger() { std::cout << "Logger down\n"; }
};

Logger logger;                  // constructed before main, destroyed after main

int main() { globalCounter++; }

// c) `static` class members

class Config {
public:
	static int instances;       // one shared variable for the whole class
};
int Config::instances = 0;      // defined once


//Key traits :
//-Lifetime = entire program(from initialization to program termination).
//- Exactly * *one instance**, regardless of how many times the block / class is used.
//- Zero - initialized by default if not explicitly initialized.
//- Caveat : **initialization / destruction order * *across translation units is unspecified(the "static initialization order fiasco") — prefer function - local statics(Meyers singleton) to control it.
//
// 3. Dynamic objects — on the heap

//Allocated * *explicitly * *with `new` (or `malloc`) at runtime, and live until * *you * *free them with `delete` (or until the program ends).This is the only storage duration you control manually.

#include <memory>
void f() {
	int* p = new int(42);          // dynamic object, on the heap
	std::cout << *p << "\n";
	delete p;                      // YOU must free it — otherwise a memory leak
	// Better: RAII-managed dynamic object
	auto up = std::make_unique<int>(7);   // dynamic, but freed automatically
}                                          // up's destructor calls delete

int main() {
	Shape* s = new Circle(2);      // dynamic, polymorphic
	// ...
	delete s;                      // virtual destructor frees correctly
}
//
//Key traits :
//-Lifetime = from `new` to `delete` — * *not**tied to scope.
//- Size / type can be decided at runtime.
//- Survives the function that created it(that's why it's used for returning objects / polymorphic ownership).
//- Must be freed manually, or wrapped in a smart pointer(`unique_ptr`/`shared_ptr`) to avoid * *memory leaks * *.
//- Slightly slower : heap allocation + possible fragmentation.

// Side - by - side comparison

#include <iostream>

struct Tracer {
	std::string name;
	Tracer(std::string n) : name(std::move(n)) { std::cout << "+ " << name << "\n"; }
	~Tracer() { std::cout << "- " << name << "\n"; }
};

Tracer globalObj("global");       // STATIC: constructed before main

void demo() {
	Tracer autoObj("automatic");  // AUTOMATIC: constructed each call, destroyed on exit
	static Tracer staticObj("static-local");   // STATIC: constructed once, destroyed at exit
	Tracer* dynObj = new Tracer("dynamic");    // DYNAMIC: manual lifetime
	delete dynObj;                             // freed here
}

int main() {
	demo();
	demo();     // note: staticObj is NOT constructed again
}
// globalObj destroyed after main

//Output:
//+ global
//+ automatic
//+ static - local
//+ dynamic
//- dynamic
//- automatic
//+ automatic          ← second call : automatic again, static - local NOT
//+ dynamic
//- dynamic
//- automatic
//- static - local        ← static locals destroyed at program end
//- global

//## Summary
//- Automatic * *objects live on the stack, tied to a scope — created and destroyed automatically, the default for locals.
//- Static * *objects live for the whole program — one shared instance, initialized once(local `static`, globals, static members).
//- Dynamic * *objects live on the heap — created with `new`, freed with `delete`, lifetime under your control, and the main source of leaks if mismanaged.
//
// Rule of thumb: prefer automatic objects; use `static` for genuinely program - wide state; 
// use dynamic allocation only when you need runtime - sized / polymorphic objects or a lifetime 
// beyond the creating scope — and then wrap it in a smart pointer(`unique_ptr`/`shared_ptr`) 
// so it's freed via RAII.