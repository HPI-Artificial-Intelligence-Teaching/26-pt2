TARGETS = composition
CXX = clang++
CPPFLAGS = -std=c++17 -g

all: $(TARGETS)

composition: composition.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS)
	$(RM) -r $(addsuffix .dSYM,$(TARGETS))

.PHONY: all clean
