//8. Is the pointer incremented(++) and decremented(--) relative to its Base81 type?
// Pointer `++` / `--` and the Base81 type

//Yes.When you increment or decrement a pointer, the address moves by
// `sizeof` the pointed - to type— not by 1 byte.The pointer's static type determines the step size.

#include <iostream>
void q81() {
    int    arr[3] = { 10, 20, 30 };
    int* p = arr;

    std::cout << "p     = " << p << "\n";
    std::cout << "*p    = " << *p << "\n";   // 10

    ++p;// advances by sizeof(int)
    std::cout << "p     = " << p << "\n";
    std::cout << "*p    = " << *p << "\n";   // 20

    --p;// back by sizeof(int)
    std::cout << "*p    = " << *p << "\n";   // 10
}
//On a typical machine with 4 - byte `int`, `p` jumps by 4 bytes each step.
// With `double*` it would jump by 8, with `char*` by 1, and so on — the step is always `sizeof(* p)`

// The step depends on the* static* type
//This is the key subtlety: `++ ` uses the pointer's declared type, not the dynamic type of the 
// object it points to

struct Base81 { int b; };
struct Derived81 : Base81 { int d; };   // larger than Base81

Derived81 arr[3];
Derived81* dp = arr;
++dp;// advances by sizeof(Derived81) — correct

Base81* bp = arr;// upcast: points to arr[0]
++bp;// advances by sizeof(Base81) — NOT sizeof(Derived81)!

//Here `++bp` moves only `sizeof(Base81)` bytes.
//Since `Derived81` objects are bigger, `bp` no longer points to `arr[1]` — it points into 
// the middle of `arr[0]`. Dereferencing it is undefined behavior.
//Rule: pointer arithmetic is only valid when the pointer's static type matches the actual 
// array element type. Upcasting a pointer to an array and then doing `++`/`--` breaks that.

//What about `void*` and `char*`?

//`char*` / `unsigned char* `: step = 1 byte(useful for byte - level walking).
//`void*`: pointer arithmetic is not allowed in standard C++ (`++` on `void*` is an error).
// Use `char*` or `std::byte* ` if you need byte stepping.

void* vp = arr;
// ++vp;// error: arithmetic on 'void*'
char* cp = reinterpret_cast<char*>(arr);
++cp;// OK: advances by 1 byte


// Summary

//So: **yes, `++`/`--` are relative to the Base(pointed - to) type — specifically to 
// `sizeof( * ptr)` as determined by the pointer's static type.