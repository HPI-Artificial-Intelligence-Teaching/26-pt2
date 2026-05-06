TARGETS = token_parsing rule_of_three
CXX = clang++
CPPFLAGS = -std=c++17 -g

all: $(TARGETS)

token_parsing: token_parsing.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

rule_of_three: rule_of_three.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS)
	$(RM) -r $(addsuffix .dSYM,$(TARGETS))

.PHONY: all clean
