TARGETS = fib

CXX = g++
CPPFLAGS = -std=c++17 -O3
LDLIBS=-lm

all: $(TARGETS)

fib: fib.cpp
	$(CXX) $(CPPFLAGS) $< -o $@

clean:
	$(RM) $(TARGETS)

.PHONY: all clean