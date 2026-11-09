//12. Is it determined by the type of the addressable object or by the type of the pointer itself, which version of the virtual function will be executed ?
//Which version runs : the object's type or the pointer's type ?

// It's determined by the dynamic type of the object 
// being pointed to — not by the static type of the pointer. 
// The pointer's type only decides whether the call is dispatched virtually at all; 
// once it is, the object's actual type picks the function.

//Two conditions must both hold for dynamic dispatch :
//1. The function is declared `virtual` (in some Base121 class).
//2. The call goes through a * *pointer or reference * *to a Base121.

//When both hold, the runtime type of the* object* decides.Otherwise the pointer's static type decides at compile time.

#include <iostream>
class Base121 {
public:
    virtual ~Base121() = default;
    virtual void f() const { std::cout << "Base121::f\n"; }
    void g() const { std::cout << "Base121::g (non-virtual)\n"; }
};

class Derived121 : public Base121 {
public:
    void f() const override { std::cout << "Derived121::f\n"; }
    void g() const { std::cout << "Derived121::g (hides)\n"; }
};

void q121() {
    Derived121 d;

    // Pointer's static type is Base121*, object's dynamic type is Derived121
    Base121* bp = &d;

    bp->f();     // "Derived121::f"  ← object's type wins (virtual)
    bp->g();     // "Base121::g"     ← pointer's type wins (non-virtual)

    // Same object, pointer of its own type
    Derived121* dp = &d;
    dp->f();     // "Derived121::f"
    dp->g();     // "Derived121::g"
}

//The object is the same in all four calls.
// The difference is entirely about whether the call is virtual
//`f` is virtual → the object's dynamic type (`Derived121`) selects `Derived121::f`,
// even through a `Base121*`.
//`g` is non - virtual → the pointer's static type (`Base121*`) selects `Base121::g`, 
// even though the object is a `Derived121`.

//Why "the pointer's type" still matters?
//The pointer's static type has two effects:
//1. It decides whether dispatch is virtual at all. 
// Calling through `Base*` where `f` is virtual → dynamic dispatch.
// Calling `Derived::g()` directly(non - virtual) → compile - time choice.
//2. It decides which functions are visible / accessible.
// A `Base*` can only call `Base`'s interface.
// `bp- > DerivedOnlyMethod()` won't compile even though the object is a `Derived`.

Base121 * bp = &d;
// bp->g();            // OK — g exists in Base121 (static resolution to Base121::g)
// bp->Derived121Only();  // error: not a member of Base121, regardless of dynamic type

// References behave the same way

void byRef(Base121 & b) { b.f(); }   // virtual → object's type wins
void byValue(Base121 b) { b.f(); }   // sliced copy: object IS a Base121 now

Derived121 d;
byRef(d);     // Derived121::f   (dynamic type preserved through reference)
byValue(d);   // Base121::f      (sliced: the copy's type is Base121)


//By - value passing slices away the Derived part, so the "object type" 
// is genuinely `Base` inside the function — another reminder that dispatch follows the object, 
// and references / pointers are what preserve the object's real type.

//Bottom line : for virtual calls through a pointer or reference, the addressable object's dynamic 
// type determines which override executes. 
// The pointer's type only gates whether the call is virtual and what members you can name 
// — it does not pick the function for virtual calls.