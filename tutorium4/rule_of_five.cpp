// Tutorial: The Rule of Five
//
// Recap from Tutorium 2 (rule_of_three.cpp): a class that manages a raw
// heap resource needs a custom destructor, copy constructor, and
// copy-assignment operator — otherwise the compiler-generated versions do a
// shallow copy and you get a double-free.
//
// C++11 added *move semantics*: an object can be constructed/assigned not by
// copying another object's resource, but by *stealing* it. This turns an
// O(n) deep copy into an O(1) pointer swap. If your class needs the Rule of
// Three, it usually also wants:
//
//   4. Move constructor      T(T&& other)
//   5. Move-assignment        T& operator=(T&& other)
//
// This is the "Rule of Five". A moved-from object must be left in a state
// that is still safe to (a) destruct and (b) optionally reuse — typically by
// resetting its pointer to nullptr (delete nullptr is a no-op).
//
// Compile: g++ -std=c++17 -o rule_of_five rule_of_five.cpp
// Run:     ./rule_of_five

#include <iostream>
#include <utility>  // std::move

// ---------------------------------------------------------------------------
// Buffer: owns a heap-allocated int array. Unlike the single `int*` from
// rule_of_three.cpp, copying this is an O(n) operation — which makes the
// benefit of *moving* instead of copying actually visible.
// ---------------------------------------------------------------------------
struct Buffer
{
    int* data;
    std::size_t size;

    // 1. Constructor
    Buffer(std::size_t n) : data(new int[n]), size(n)
    {
        for (std::size_t i = 0; i < size; i++) data[i] = static_cast<int>(i);
        std::cout << "[Buffer] constructor, size=" << size << std::endl;
    }

    // 2. Destructor
    ~Buffer()
    {
        std::cout << "[Buffer] destructor, size=" << size << std::endl;
        delete[] data;  // delete[] nullptr is safe (no-op)
    }

    // 3a. Copy constructor — deep copy, O(n)
    Buffer(const Buffer& other) : data(new int[other.size]), size(other.size)
    {
        std::cout << "[Buffer] copy constructor, deep-copying " << size << " ints" << std::endl;
        for (std::size_t i = 0; i < size; i++) data[i] = other.data[i];
    }

    // 3b. Copy-assignment operator — deep copy, O(n)
    Buffer& operator=(const Buffer& other)
    {
        if (this == &other) return *this;

        std::cout << "[Buffer] copy assignment, deep-copying " << other.size << " ints" << std::endl;
        delete[] data;
        size = other.size;
        data = new int[size];
        for (std::size_t i = 0; i < size; i++) data[i] = other.data[i];
        return *this;
    }

    // 4. Move constructor — steal the pointer, O(1).
    // `other` is left in a valid-but-empty state (nullptr, size 0), so its
    // destructor runs safely and `other` could even be reused afterwards.
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size)
    {
        std::cout << "[Buffer] move constructor, stealing buffer of size=" << size << std::endl;
        other.data = nullptr;
        other.size = 0;
    }

    // 5. Move-assignment operator — free our own resource, then steal.
    Buffer& operator=(Buffer&& other) noexcept
    {
        if (this == &other) return *this;

        std::cout << "[Buffer] move assignment, stealing buffer of size=" << other.size << std::endl;
        delete[] data;       // free what we currently own
        data = other.data;   // steal
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        return *this;
    }
};

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------
int main()
{
    std::cout << "--- copy: deep-copies the array ---" << std::endl;
    Buffer a(5);
    Buffer b = a;  // copy constructor
    std::cout << "a.size=" << a.size << "  b.size=" << b.size << std::endl;

    std::cout << "\n--- move: steals the array, no copying ---" << std::endl;
    Buffer c = std::move(a);  // move constructor
    std::cout << "a.size=" << a.size << " (moved-from, now empty)  c.size=" << c.size << std::endl;

    std::cout << "\n--- move-assignment: frees own buffer, then steals ---" << std::endl;
    Buffer d(2);
    d = std::move(b);  // move assignment: d's old buffer (size 2) is freed first
    std::cout << "b.size=" << b.size << " (moved-from)  d.size=" << d.size << std::endl;

    std::cout << "\n--- move-assignment from a temporary (implicit, no std::move) ---" << std::endl;
    // Buffer(7) is a temporary (rvalue) — it binds directly to `Buffer&& other`,
    // so move-assignment is selected without writing std::move.
    d = Buffer(7);
    std::cout << "d.size=" << d.size << std::endl;

    std::cout << "\n--- moved-from objects are still safely destructible ---" << std::endl;
    std::cout << "End of main, all four destructors run now." << std::endl;
    return 0;
}

// ---------------------------------------------------------------------------
// Connection to A3 (Trie):
//
// The Trie class owns a `Node* root` pointing to a tree built with `new`.
// The same five members apply:
//   - destructor / copy ctor / copy assign  -> recurse over the whole tree
//     (free_tree / deep_copy_tree)
//   - move ctor / move assign -> just steal `root` and set the source's
//     `root` to nullptr (O(1), no recursion needed!)
//
// One subtlety that does NOT come up with `Buffer`: after Trie's move ctor
// sets `trie.root = nullptr`, the moved-from trie must remain *usable*, not
// just destructible — e.g. `trie.add(...)` should still work. That means
// `add` must handle `root == nullptr` (e.g. by allocating a fresh root if
// needed) rather than assuming `root` is always valid.
// ---------------------------------------------------------------------------
