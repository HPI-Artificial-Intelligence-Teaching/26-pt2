/******************************************************************************
 *
 * An undirected graph, where the vertex names are arbitrary strings.
 *
 * Based on the source code from Robert Sedgewick and Kevin Wayne at https://algs4.cs.princeton.edu/
 *
 ******************************************************************************/

#ifndef SYMBOL_GRAPH_H
#define SYMBOL_GRAPH_H

#include <string>

#include "graph.h"
#include "redblack_bst.h"


// Implements a class representing an undirected graph, where the vertex names are arbitrary strings
class SymbolGraph {
    RedBlackBST<std::string, int>* st;  // string -> index
    std::string* keys;                  // index  -> string
    Graph* graph;                  // the underlying graph

   public:
    // constructor
    SymbolGraph(const std::string&, const char);

    // copy constructor
    SymbolGraph(const SymbolGraph& sg) {
        st = new RedBlackBST<std::string, int>(*sg.st);
        graph = new Graph(*sg.graph);
        keys = new std::string[graph->V()];
        for (auto v = 0; v < graph->V(); v++) keys[v] = sg.keys[v];
    }

    // move constructor
    SymbolGraph(SymbolGraph&& sg) : st(sg.st),
                                    keys(sg.keys),
                                    graph(sg.graph) {
        sg.st = nullptr;
        sg.keys = nullptr;
        sg.graph = nullptr;
    }

    // copy assignment
    SymbolGraph& operator=(const SymbolGraph& sg) {
        // free the existing lists, map and graph
        delete st;
        delete[] keys;
        delete graph;

        // copy the symbol graph passed in
        st = new RedBlackBST<std::string, int>(*sg.st);
        graph = new Graph(*sg.graph);
        keys = new std::string[graph->V()];
        for (auto v = 0; v < graph->V(); v++) keys[v] = sg.keys[v];
        return (*this);
    }

    // move assignment
    SymbolGraph& operator=(SymbolGraph&& sg) {
        // free the existing lists, map and graph
        delete st;
        delete[] keys;
        delete graph;

        st = sg.st;
        keys = sg.keys;
        graph = sg.graph;
        sg.st = nullptr;
        sg.keys = nullptr;
        sg.graph = nullptr;
        return (*this);
    }

    // destructor
    ~SymbolGraph() {
        delete st;
        delete[] keys;
        delete graph;
    }

    // returns whether or not the graph contain the vertex named s
    bool contains(const std::string& s) const { return (st->contains(s)); }

    // returns the integer associated with the vertex named s
    int index_of(const std::string& s) const { return (*(st->get(s))); }

    // returns the name of the vertex associated with the integer
    std::string name_of(const int v) const { return (keys[v]); }

    // returns the associated graph
    const Graph* get_graph() const { return (graph); }
};

#endif