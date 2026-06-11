TARGETS = rule_of_five generic_iteration array_return static_const_cache callable_member
CXX = clang++
CPPFLAGS = -std=c++17 -g

all: $(TARGETS)

rule_of_five: rule_of_five.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

generic_iteration: generic_iteration.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

array_return: array_return.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

static_const_cache: static_const_cache.cpp
	$(CXX) $(CPPFLAGS) -O0 -o $@ $<

callable_member: callable_member.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS)
	$(RM) -r $(addsuffix .dSYM,$(TARGETS))

.PHONY: all clean
