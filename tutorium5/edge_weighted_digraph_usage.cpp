// Tutorial: Using EdgeWeightedDigraph — a directed graph with weighted edges
//
// Demonstrates:
//   - Constructing an EdgeWeightedDigraph with a fixed number of vertices and
//     adding edges afterwards with add_edge(Edge(from, to, weight))
//   - Edge as an immutable value object: from() / to() / weight()
//   - adj(v) yields Edge objects, not ints — the neighbour index is e.to()
//   - Parallel edges are allowed (and present in the exercise data!) —
//     finding the cheapest of several edges between the same two vertices
//   - add_edge() validates indices and throws "Vertex index out of range"
//
// Relevance: This is the graph INSIDE your WeightedStructDigraph (Aufgabe 1.1).
// Its shape dictates your class design: the vertex count is fixed at
// construction (so all nodes must be known first — that is why the
// constructor takes a Bag<Node>), while edges can be added afterwards (that
// is what your add_edge method wraps, translating Node -> index).
// In Aufgabe 2.1 the neighbour loop of A* runs over exactly these Edge
// objects, and in Aufgabe 2.2 the travel time of a path hop is the weight of
// the CHEAPEST parallel edge between two consecutive airports.
//
// Compile: clang++ -std=c++17 -o edge_weighted_digraph_usage edge_weighted_digraph_usage.cpp vlib/weighted_digraph.cpp
// Run:     ./edge_weighted_digraph_usage

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "vlib/weighted_digraph.h"

int main()
{
    // The same idea as in symbol_digraph_usage.cpp, one layer further down:
    // the graph itself only knows indices 0..V-1. Here WE maintain the
    // mapping by hand; in Aufgabe 1.1 your WeightedStructDigraph does it
    // with a RedBlackBST<Node, int> and a Node* array.
    const std::string city[4] = {"Hamburg", "Berlin", "Frankfurt", "Paris"};

    // ------------------------------------------------------------------
    // 1. Construction: V is fixed up front, edges come later
    // ------------------------------------------------------------------
    // There is no way to add a vertex afterwards — the adjacency lists are
    // allocated once. For the exercise this means: first load ALL airports
    // (the Bag<Node> in your constructor), then add the routes as edges.
    EdgeWeightedDigraph g(4);

    // An Edge is a small immutable value: (from, to, weight). Weights are
    // travel times in hours here — just like the Time column of routes.csv.
    g.add_edge(Edge(0, 1, 1.75));   // Hamburg   -> Berlin     ICE       1.75h
    g.add_edge(Edge(0, 1, 2.9));    // Hamburg   -> Berlin     FlixTrain 2.9h  (parallel edge!)
    g.add_edge(Edge(1, 0, 1.75));   // Berlin    -> Hamburg
    g.add_edge(Edge(0, 2, 3.4));    // Hamburg   -> Frankfurt
    g.add_edge(Edge(1, 2, 4.0));    // Berlin    -> Frankfurt
    g.add_edge(Edge(2, 3, 3.8));    // Frankfurt -> Paris

    std::cout << "vertices: " << g.V() << ", edges: " << g.E() << std::endl;

    // ------------------------------------------------------------------
    // 2. The neighbour loop: adj(v) yields Edges, not ints
    // ------------------------------------------------------------------
    // Compare with Digraph, where adj(v) gives you the neighbour indices
    // directly. Here each element is an Edge; the neighbour is e.to() and
    // the cost of going there is e.weight(). This is precisely the loop at
    // the heart of A* ("Für jeden Nachbarn n_k von v_i ...").
    // (Bag iterates in reverse insertion order — see symbol_digraph_usage.cpp.)
    std::cout << "connections from Hamburg:" << std::endl;
    for (Edge& e : g.adj(0)) {
        std::cout << "  " << city[e.from()] << " -> " << city[e.to()]
                  << " (" << e.weight() << "h)" << std::endl;
    }

    // ------------------------------------------------------------------
    // 3. Parallel edges: pick the cheapest connection between two vertices
    // ------------------------------------------------------------------
    // Hamburg -> Berlin exists twice with different weights. The graph
    // stores both; if you want "the" travel time between two consecutive
    // vertices of a path (Aufgabe 2.2), you must scan the adjacency list
    // and keep the minimum yourself.
    double best = std::numeric_limits<double>::infinity();
    for (Edge& e : g.adj(0)) {
        if (e.to() == 1 && e.weight() < best) {
            best = e.weight();
        }
    }
    std::cout << "fastest Hamburg -> Berlin: " << best << "h" << std::endl;   // 1.75h

    // ------------------------------------------------------------------
    // 4. add_edge() validates its endpoints
    // ------------------------------------------------------------------
    // Both endpoints must be in 0..V-1, otherwise add_edge throws. In
    // Aufgabe 1.2 you would hit this if a route referenced an airport that
    // was not loaded as a vertex first.
    try {
        g.add_edge(Edge(0, 7, 1.0));   // there is no vertex 7
    } catch (const std::logic_error& e) {
        std::cout << "caught: " << e.what() << std::endl;
    }

    // ------------------------------------------------------------------
    // 5. Printing the whole graph
    // ------------------------------------------------------------------
    // EdgeWeightedDigraph has an operator<< — handy for debugging your
    // load_data: does the graph actually contain what you think it does?
    std::cout << g;

    return 0;
}

// Things to try / predict before running:
//  (a) Extend step 3 to the whole path Hamburg -> Berlin -> Frankfurt -> Paris:
//      sum the cheapest hop weights. (This is the output loop of your CLI.)
//  (b) What is the weight of Edge(0, 1)? (Look at the default arguments of
//      the Edge constructor in vlib/edge.h — and think about why infinity is
//      a sensible default for a "connection that was never specified".)
//  (c) Edge has comparison operators. Which field(s) do they compare?
//      Predict: Edge(0, 1, 2.0) == Edge(2, 3, 2.0) — true or false?
