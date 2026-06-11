// Tutorial: Generic iteration over a templated container
//
// A function template doesn't need to know the concrete container type —
// only that it can be iterated with a range-based for loop and that its
// elements can be converted to integers. This is the core idea behind
// A2.1 (sum_hash / rolling_hash): they take a `const T&` and iterate
// over it without caring whether T is std::string, std::vector<int>, or
// std::array<char, N>.
//
// Compile: g++ -std=c++17 -o generic_iteration generic_iteration.cpp
// Run:     ./generic_iteration

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

// Works for any T whose elements can be cast to unsigned char, e.g.
// std::string (elements are char) or std::vector<int> (elements are int).
template <typename T>
std::uint64_t sum_bytes(const T& container)
{
    std::uint64_t sum = 0;
    for (const auto& element : container) {
        sum += static_cast<unsigned char>(element);
    }
    return sum;
}

int main()
{
    std::string s = "ACGT";
    std::vector<int> v = {65, 67, 71, 84};  // ASCII codes for 'A', 'C', 'G', 'T'

    std::cout << "sum_bytes(\"ACGT\")            = " << sum_bytes(s) << std::endl;
    std::cout << "sum_bytes({65, 67, 71, 84}) = " << sum_bytes(v) << std::endl;
    // Same result, even though one is iterating chars and the other ints —
    // the template body doesn't care.

    return 0;
}
