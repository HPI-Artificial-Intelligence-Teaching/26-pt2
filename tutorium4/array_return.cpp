// Tutorial: Returning a std::array<T, N> from a template function with a
// non-type template parameter
//
// `N` here is not a type — it's a compile-time constant (a "non-type
// template parameter"). The compiler generates one function per value of N
// you actually use, and the size of the returned std::array is fixed at
// compile time. This is the syntax needed for hash_keys_iterative,
// hash_keys_polynomial, and hash_keys_polynomial_primes in A2.2,
// which all return `std::array<std::uint64_t, N>`.
//
// Compile: g++ -std=c++17 -o array_return array_return.cpp
// Run:     ./array_return

#include <array>
#include <iostream>

// Returns an array of N squares, starting at `start`:
// {start^2, (start+1)^2, ..., (start+N-1)^2}
template <std::size_t N>
std::array<int, N> make_squares(int start)
{
    std::array<int, N> result{};
    for (std::size_t i = 0; i < N; i++) {
        int value = start + static_cast<int>(i);
        result[i] = value * value;
    }
    return result;
}

int main()
{
    // N is given explicitly as a template argument — the compiler needs to
    // know the array size at compile time.
    std::array<int, 3> squares3 = make_squares<3>(1);
    std::array<int, 5> squares5 = make_squares<5>(0);

    std::cout << "make_squares<3>(1) = ";
    for (int x : squares3) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "make_squares<5>(0) = ";
    for (int x : squares5) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}
