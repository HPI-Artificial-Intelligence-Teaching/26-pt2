// Tutorial: Composition over Inheritance — wrapping an object via a reference member
//
// Key concepts:
//   - Holding another object by reference as a class member
//   - Constructor initializer list (the only way to initialise a reference member)
//   - Delegation: forwarding calls to the wrapped object instead of inheriting from it
//
// Relevance: CountingComparator in Aufgabe 1 holds a Comparator& and wraps every
// compare() call. The same pattern is used here with a Printer/LoggingPrinter pair.
//
// Compile: clang++ -std=c++17 -o composition composition.cpp
// Run:     ./composition

#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// The "inner" class — does the real work
// ---------------------------------------------------------------------------
class Printer
{
public:
    void print(const std::string& msg)
    {
        std::cout << msg << "\n";
    }
};

// ---------------------------------------------------------------------------
// Wrapper: adds call-counting without touching Printer or subclassing it
// ---------------------------------------------------------------------------
class LoggingPrinter
{
    Printer& printer;   // reference member — must be initialised, never re-seated
    int callCount;

public:
    // The initialiser list ": printer(p), callCount(0)" is the ONLY way to
    // initialise a reference member.  Assigning in the body ("this->printer = p")
    // is a compile error: references cannot be assigned after construction.
    LoggingPrinter(Printer& p) : printer(p), callCount(0) {}

    void print(const std::string& msg)
    {
        ++callCount;
        printer.print(msg);   // delegate — no logic is duplicated
    }

    int getCallCount() const { return callCount; }
    void resetCallCount()    { callCount = 0; }
};

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------
int main()
{
    Printer p;
    LoggingPrinter lp(p);

    lp.print("hello");
    lp.print("world");
    std::cout << "calls so far: " << lp.getCallCount() << "\n";   // 2

    lp.resetCallCount();
    lp.print("after reset");
    std::cout << "calls so far: " << lp.getCallCount() << "\n";   // 1

    return 0;
}
