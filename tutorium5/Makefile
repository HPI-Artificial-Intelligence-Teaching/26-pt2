TARGETS = symbol_digraph_usage edge_weighted_digraph_usage index_min_pq_usage parse_csv_line
CXX = clang++
CPPFLAGS = -std=c++17 -g

all: $(TARGETS)

symbol_digraph_usage: symbol_digraph_usage.cpp vlib/symbol_digraph.cpp vlib/digraph.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^

edge_weighted_digraph_usage: edge_weighted_digraph_usage.cpp vlib/weighted_digraph.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^

index_min_pq_usage: index_min_pq_usage.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

parse_csv_line: parse_csv_line.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS)
	$(RM) -r $(addsuffix .dSYM,$(TARGETS))

.PHONY: all clean
