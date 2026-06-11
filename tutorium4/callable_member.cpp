// Tutorial: Storing a callable with std::function and calling it via a
// member
//
// std::function<R(Args...)> can hold *any* callable with a matching
// signature: a free function, a lambda, a functor object, ... A class can
// store one as a member and call it later without knowing what kind of
// callable it actually is. This is exactly the pattern needed for
// `key_function` in BloomFilter (A2.3): the filter is constructed
// with some hash function and calls it later via the stored
// std::function, without caring whether it's a free function or a lambda.
//
// Compile: g++ -std=c++17 -o callable_member callable_member.cpp
// Run:     ./callable_member

#include <functional>
#include <iostream>

class Calculator
{
   private:
    std::function<int(int, int)> op;

   public:
    Calculator(std::function<int(int, int)> op) : op(op) {}

    int apply(int a, int b)
    {
        return op(a, b);
    }
};

// A free function with the right signature.
int multiply(int a, int b)
{
    return a * b;
}

int main()
{
    // 1. Store a free function.
    Calculator multiplier(multiply);
    std::cout << "multiplier.apply(3, 4) = " << multiplier.apply(3, 4) << std::endl;

    // 2. Store a lambda.
    Calculator adder([](int a, int b) { return a + b; });
    std::cout << "adder.apply(3, 4) = " << adder.apply(3, 4) << std::endl;

    // 3. Store a lambda that captures state.
    int offset = 10;
    Calculator offsetter([offset](int a, int b) { return a + b + offset; });
    std::cout << "offsetter.apply(3, 4) = " << offsetter.apply(3, 4) << std::endl;

    // The Calculator class itself never changes — only the callable passed
    // to its constructor differs. Same idea as plugging different hash
    // functions into BloomFilter.
    return 0;
}
