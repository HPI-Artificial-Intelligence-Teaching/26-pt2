// Tutorial: The Rule of Three
//
// When a class manages a raw heap resource (via new/delete), the compiler-
// generated copy constructor and copy-assignment operator do a *shallow copy*:
// they copy the pointer value, not the data it points to.  Two objects then
// share the same memory, and when both destructors run, the memory is freed
// twice — undefined behavior, usually a crash.
//
// The Rule of Three says: if you need a custom destructor, you almost
// certainly also need a custom copy constructor and copy-assignment operator.
//
// Compile: g++ -std=c++17 -o rule_of_three rule_of_three.cpp
// Run:     ./rule_of_three

#include <iostream>

// ---------------------------------------------------------------------------
// BAD: relies on compiler-generated copy — leads to double-free
// ---------------------------------------------------------------------------
struct Broken
{
    int* value;

    Broken(int v) : value(new int(v))
    {
        std::cout << "[Broken] constructor, value=" << *value << std::endl;
    }

    ~Broken()
    {
        std::cout << "[Broken] destructor, freeing value=" << *value << std::endl;
        delete value;   // if two objects share this pointer, second delete crashes
    }

    // No copy constructor  → compiler generates:  value(other.value)
    // No copy assignment   → compiler generates:  this->value = other.value
    // Both copy the *pointer*, not the int it points to.
};

// ---------------------------------------------------------------------------
// GOOD: Rule of Three implemented correctly
// ---------------------------------------------------------------------------
struct Fixed
{
    int* value;

    // 1. Constructor
    Fixed(int v) : value(new int(v))
    {
        std::cout << "[Fixed] constructor, value=" << *value << std::endl;
    }

    // 2. Destructor — frees the resource
    ~Fixed()
    {
        std::cout << "[Fixed] destructor, freeing value=" << *value << std::endl;
        delete value;
    }

    // 3a. Copy constructor — allocates *new* memory and copies the content
    Fixed(const Fixed& other) : value(new int(*other.value))
    {
        std::cout << "[Fixed] copy constructor, copied value=" << *value << std::endl;
    }

    // 3b. Copy-assignment operator
    Fixed& operator=(const Fixed& other)
    {
        // Guard against self-assignment (e.g. `a = a`):
        // without this, with more complex data structures such as a linked list, 
        // we'd have to free our own memory and then read from it.
        if (this == &other)
            return *this;

        *value = *other.value;  // overwrite the content, reuse our allocation
        std::cout << "[Fixed] copy assignment, value now=" << *value << std::endl;
        return *this;
    }
};

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------
void demo_broken()
{
    std::cout << "\n--- Broken (shallow copy) ---" << std::endl;
    Broken a(10);
    Broken b = a;           // copy constructor: b.value == a.value (same pointer!)
    std::cout << "a.value=" << *a.value << "  b.value=" << *b.value << std::endl;
    *b.value = 99;          // modifying b also changes a — they share memory
    std::cout << "After setting b to 99:" << std::endl;
    std::cout << "a.value=" << *a.value << "  b.value=" << *b.value << std::endl;
    // When the function returns, both destructors run → double-free crash!
    // Comment out this call in main() to avoid the crash and see Fixed instead.
}

void demo_fixed()
{
    std::cout << "\n--- Fixed (deep copy, Rule of Three) ---" << std::endl;
    Fixed a(10);
    Fixed b = a;            // copy constructor: b.value points to NEW int
    std::cout << "a.value=" << *a.value << "  b.value=" << *b.value << std::endl;
    *b.value = 99;          // only b changes
    std::cout << "After setting b to 99:" << std::endl;
    std::cout << "a.value=" << *a.value << "  b.value=" << *b.value << std::endl;

    Fixed c(0);
    c = a;                  // copy-assignment operator
    std::cout << "After c = a:  c.value=" << *c.value << std::endl;

    c = c;                  // self-assignment — must not crash or corrupt
    std::cout << "After c = c (self):  c.value=" << *c.value << std::endl;
    // All three destructors run safely — each has its own allocation.
}

int main()
{
    // Uncomment demo_broken() only to observe the shallow-copy problem.
    // It will likely crash when both destructors run at end of the function.
    // demo_broken();

    demo_fixed();
    std::cout << "\nEnd of main, destructors will run now." << std::endl;
    return 0;
}
