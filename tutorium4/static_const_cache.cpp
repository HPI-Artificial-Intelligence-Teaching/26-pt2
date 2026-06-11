// Tutorial: "Compute once per template instantiation" with static const
//
// A `static const` variable inside a function is initialized exactly once —
// the first time control reaches its declaration — and that value is reused
// on every later call. For a *template* function, this happens once per
// distinct template instantiation (i.e., once per N).
//
// This is the trick hinted at for hash_keys_polynomial_primes in A2.2:
// filling an std::array<..., N> with some values can be expensive, but
// if it's stored in a `static const std::array<..., N>`, it only happens
// once no matter how many times the function is called afterwards. We time
// the calls below to make the effect visible.
//
// Compile: g++ -std=c++17 -O0 -o static_const_cache static_const_cache.cpp
// Run:     ./static_const_cache
//
// (Use -O0 so the compiler doesn't optimize the "expensive" computation
// away entirely — with -O2 the difference may shrink further.)

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>

// Deliberately naive (exponential-time, no memoization) Fibonacci, so that
// filling the array below is slow enough to measure.
std::uint64_t naive_fib(int n)
{
    if (n < 2) return static_cast<std::uint64_t>(n);
    return naive_fib(n - 1) + naive_fib(n - 2);
}

template <std::size_t N>
std::array<std::uint64_t, N> compute_fibonacci()
{
    std::array<std::uint64_t, N> fib{};
    for (std::size_t i = 0; i < N; i++) {
        fib[i] = naive_fib(static_cast<int>(i));
    }
    return fib;
}

template <std::size_t N>
std::uint64_t fibonacci_sum()
{
    // Computed only once per N, even though fibonacci_sum<N>() may be
    // called many times.
    static const std::array<std::uint64_t, N> fib = compute_fibonacci<N>();

    std::uint64_t sum = 0;
    for (auto f : fib) sum += f;
    return sum;
}

int main()
{
    constexpr std::size_t N = 32;

    auto t0 = std::chrono::steady_clock::now();
    std::uint64_t sum1 = fibonacci_sum<N>();
    auto t1 = std::chrono::steady_clock::now();
    std::uint64_t sum2 = fibonacci_sum<N>();
    auto t2 = std::chrono::steady_clock::now();

    constexpr std::size_t M = 34;
    std::uint64_t sum3 = fibonacci_sum<M>();
    auto t3 = std::chrono::steady_clock::now();

    auto first_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto second_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto third_us = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();

    std::cout << "First call to fibonacci_sum<" << N << ">():  sum=" << sum1
              << ", took " << first_us << " us (fills the array)" << std::endl;
    std::cout << "Second call to fibonacci_sum<" << N << ">(): sum=" << sum2
              << ", took " << second_us << " us (reuses cached array)" << std::endl;
    std::cout << "First call to fibonacci_sum<" << M << ">():  sum=" << sum3
              << ", took " << third_us << " us (new instantiation, fills again)" << std::endl;

    return 0;
}
