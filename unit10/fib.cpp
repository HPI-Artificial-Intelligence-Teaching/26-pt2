/******************************************************************************
 *
 *  Fibonacci numbers with and without memoization
 *
 *  % ./time fib 50
 *
 ******************************************************************************/

#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

// Fibonacci numbers without memoization
long long fib(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

// Fibonacci numbers with memoization
long long fib_fast(int n, long long* memo) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (memo[n] == -1)
        memo[n] = fib_fast(n - 1, memo) + fib_fast(n - 2, memo);
    return memo[n];
}

#define N 1000

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n>" << endl;
        return (1);
    }
    auto n = atoi(argv[1]);
    if (n >= N) {
        cerr << "n must be less than " << N << endl;
        return (1);
    }

    // memoized version
    long long memo[N];
    for (auto i = 0; i < N; i++)
        memo[i] = -1;

    auto start_fast = clock();
    auto result_fast = fib_fast(n, memo);
    auto time_fast = double(clock() - start_fast) / double(CLOCKS_PER_SEC);
    cout << "fib_fast(" << n << ") = " << result_fast
         << " (" << setprecision(4) << time_fast << "s)" << endl;

    // naive version (only for small n to avoid excessive wait)
    if (n <= 45) {
        auto start_slow = clock();
        auto result_slow = fib(n);
        auto time_slow = double(clock() - start_slow) / double(CLOCKS_PER_SEC);
        cout << "fib(" << n << ") = " << result_slow
             << " (" << setprecision(4) << time_slow << "s)" << endl;
    } else {
        cout << "fib(" << n << ") skipped (too slow without memoization)" << endl;
    }

    return (0);
}