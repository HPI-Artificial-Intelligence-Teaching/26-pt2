// Tutorial: Using SymbolDigraph — a directed graph whose vertices have names
//
// Demonstrates:
//   - Constructing a SymbolDigraph from a delimited text file
//   - The two-way mapping between names and indices:
//       index_of (string -> int)  and  name_of (int -> string)
//   - The internal Digraph only knows ints 0..V-1 — names exist only at the
//     boundary of the class
//   - Iterating over neighbours with adj() and translating back to names
//   - outdegree() / indegree()
//   - PITFALL: index_of() on an unknown name is undefined behaviour — always
//     check contains() first
//
// Relevance: In Aufgabe 1.1 you implement WeightedStructDigraph, which follows
// exactly this design — but the vertices are values of a generic struct type
// `Node` instead of std::string, and the internal graph is an
// EdgeWeightedDigraph instead of a Digraph. Every method you see here
// (contains / index_of / name_of / get_graph) has a counterpart there.
// The "translate at the boundary" pattern is also the key to Aufgabe 2:
// A* itself only ever works on indices; structs go in and come out.
//
// Compile: clang++ -std=c++17 -o symbol_digraph_usage symbol_digraph_usage.cpp vlib/symbol_digraph.cpp vlib/digraph.cpp
// Run:     ./symbol_digraph_usage        (from this folder, so trains.txt is found)

#include <iostream>
#include <string>

#include "vlib/symbol_digraph.h"

int main()
{
    // trains.txt models direct train connections. One line per departure
    // station: "Hamburg,Berlin,Frankfurt" means Hamburg->Berlin and
    // Hamburg->Frankfurt. Stations that never appear as a first token
    // (here: Wien) still become vertices — they just have no outgoing edges.
    //
    // Worth reading once: vlib/symbol_digraph.cpp. The constructor reads the
    // file THREE times — pass 1 gives every distinct name an index in the
    // RedBlackBST (name -> int), pass 2 fills the keys[] array (int -> name),
    // pass 3 adds the edges to a Digraph that only stores ints.
    // Your WeightedStructDigraph keeps the same three ingredients:
    // a RedBlackBST<Node, int>, a Node* array, and the internal graph.
    // BUT: the constructor for the WeightedStructDigraph may look different!
    SymbolDigraph g("trains.txt", ',');

    const Digraph* graph = g.get_graph();
    std::cout << "vertices: " << graph->V() << ", edges: " << graph->E() << std::endl;

    // ------------------------------------------------------------------
    // 1. The two-way mapping: names <-> indices
    // ------------------------------------------------------------------
    // The order of the indices is simply the order in which the names were
    // first seen in the file — do not rely on it, always translate.
    int berlin = g.index_of("Berlin");
    std::cout << "index_of(\"Berlin\") = " << berlin << std::endl;
    std::cout << "name_of(" << berlin << ")   = " << g.name_of(berlin) << std::endl;

    // ------------------------------------------------------------------
    // 2. contains() — ALWAYS check before index_of()
    // ------------------------------------------------------------------
    // index_of() dereferences the symbol-table lookup without checking for
    // nullptr: calling g.index_of("Rom") here would be undefined behaviour
    // (in practice: a crash). The class gives you contains() so that YOU can
    // guard the lookup. Remember this for your CLI in Aufgabe 2.2, where the
    // user may type an IATA code that is not in the data.
    std::cout << "contains(\"Rom\")    = " << g.contains("Rom") << std::endl;    // 0
    std::cout << "contains(\"Berlin\") = " << g.contains("Berlin") << std::endl; // 1

    // ------------------------------------------------------------------
    // 3. Neighbours: adj() returns indices, not names
    // ------------------------------------------------------------------
    // The internal Digraph has no idea that vertex 3 is called "Frankfurt".
    // adj(v) hands you a Bag<int> of vertex indices; if you want names, you
    // translate each one back with name_of(). (In Aufgabe 2 the same loop
    // runs over weighted Edge objects, and you use e.to() as the index.)
    // By the way: Bag::add() prepends, so a Bag iterates in REVERSE insertion
    // order — Frankfurt's neighbours print as "Paris, Stuttgart" even though
    // the file lists Stuttgart first. Keep this in mind whenever you collect
    // results in a Bag (e.g. the path in Aufgabe 2.1).
    int frankfurt = g.index_of("Frankfurt");
    std::cout << "direct connections from Frankfurt:" << std::endl;
    for (int w : graph->adj(frankfurt)) {
        std::cout << "  " << frankfurt << " -> " << w
                  << "  (" << g.name_of(w) << ")" << std::endl;
    }

    // ------------------------------------------------------------------
    // 4. The whole graph, translated back to names
    // ------------------------------------------------------------------
    // Vertex indices are always exactly 0..V()-1, so a plain for loop
    // enumerates every vertex.
    std::cout << "all connections:" << std::endl;
    for (int v = 0; v < graph->V(); v++) {
        std::cout << "  " << g.name_of(v) << ":";
        for (int w : graph->adj(v)) {
            std::cout << " " << g.name_of(w);
        }
        std::cout << std::endl;
    }

    // ------------------------------------------------------------------
    // 5. Degrees
    // ------------------------------------------------------------------
    // outdegree(v) = number of outgoing edges = adj(v).size().
    // Wien only ever appears as a destination, so its outdegree is 0 —
    // a train to Wien is a dead end in this network.
    int wien = g.index_of("Wien");
    std::cout << "outdegree(Frankfurt) = " << graph->outdegree(frankfurt) << std::endl;
    std::cout << "outdegree(Wien)      = " << graph->outdegree(wien) << std::endl;
    std::cout << "indegree(Wien)       = " << graph->indegree(wien) << std::endl;

    return 0;
}

// Things to try / predict before running:
//  (a) What does the program print for step 1 if you swap the first two lines
//      of trains.txt? (Hint: indices come from first-seen order.)
//  (b) Add the line "Wien,Budapest" to trains.txt. How do V() and E() change?
//  (c) Replace step 2's contains() calls with g.index_of("Rom") and run it.
//      What happens, and where exactly does it go wrong? (Look at index_of()
//      in vlib/symbol_digraph.h.)
