// Tutorial: Using IndexMinPQ — a min-priority queue over the indices 0..capacity-1
//
// Demonstrates:
//   - insert(index, priority), min_index(), min_key(), del_min()
//   - del_min() returns the INDEX, not the priority!
//   - contains(index) + change_key(index, priority) to update a priority
//   - PITFALL: insert() for an index that is already in the queue throws
//     "index is already in the priority queue" — the exception you WILL see
//     in Aufgabe 2.1 if you forget the contains() check
//   - INFINITY is a valid float priority
//
// Relevance: In Aufgabe 2.1 the set of open vertices V_O is an
// IndexMinPQ<float>. The *index* is the vertex index from your
// WeightedStructDigraph (which is why the queue's capacity is the number of
// vertices V), and the *priority* is the estimated total cost
// f(v) = g(v) + h(v). "Take the open vertex with the smallest f" is a single
// del_min() call. When you later find a cheaper path to a vertex that is
// already open, its priority must be UPDATED — inserting it a second time
// throws.
//
// Compile: clang++ -std=c++17 -o index_min_pq_usage index_min_pq_usage.cpp
// Run:     ./index_min_pq_usage

#include <cmath>     // INFINITY
#include <iostream>
#include <stdexcept>
#include <string>

#include "vlib/index_min_pq.h"

int main()
{
    // Scenario: a repair shop with 5 numbered jobs. The priority of a job is
    // the estimated time (in hours) until its deadline — the most urgent job
    // (smallest value) should be worked on first.
    //
    // Like the vertices of a graph, the jobs live OUTSIDE the queue; the
    // queue only ever sees their indices 0..4. This array plays the role
    // that name_of() plays for your graph:
    const std::string job[5] = {"bike", "laptop", "toaster", "espresso machine", "drone"};

    // ------------------------------------------------------------------
    // 1. Construction: capacity is fixed, indices are 0..capacity-1
    // ------------------------------------------------------------------
    // The queue allocates its arrays once; there is no resizing. Using an
    // index >= capacity is undefined behaviour. In A*, construct the queue
    // with the number of vertices of the graph: IndexMinPQ<float>(graph.V()).
    IndexMinPQ<float> open(5);

    // ------------------------------------------------------------------
    // 2. insert() and peeking at the minimum
    // ------------------------------------------------------------------
    open.insert(0, 8.0f);   // bike:    8h until deadline
    open.insert(1, 3.0f);   // laptop:  3h
    open.insert(2, 5.5f);   // toaster: 5.5h

    // min_index()/min_key() only look, del_min() removes.
    std::cout << "most urgent: " << job[open.min_index()]
              << " (" << open.min_key() << "h left)" << std::endl;   // laptop (3h)

    // ------------------------------------------------------------------
    // 3. TYPICAL BUG: inserting an index that is already in the queue
    // ------------------------------------------------------------------
    // In A* this happens when you find a SECOND, cheaper path to a vertex
    // that is already open and blindly insert() it again.
    try {
        open.insert(2, 2.0f);   // toaster is already queued!
    } catch (const std::logic_error& e) {
        // (insert() also prints the offending index before throwing.)
        std::cout << "caught: " << e.what() << std::endl;
    }

    // The correct pattern — decide between update and insert:
    if (open.contains(2)) {
        open.change_key(2, 2.0f);   // update: toaster is now the most urgent
    } else {
        open.insert(2, 2.0f);
    }
    std::cout << "most urgent now: " << job[open.min_index()]
              << " (" << open.min_key() << "h left)" << std::endl;   // toaster (2h)

    // Note: there are also decrease_key()/increase_key(), but they throw if
    // the value does not move strictly in the promised direction —
    // change_key() handles both directions and is the safe default.

    // ------------------------------------------------------------------
    // 4. INFINITY is a perfectly fine priority
    // ------------------------------------------------------------------
    // floats have a real infinity: it compares greater than every finite
    // value, so an INFINITY entry simply waits at the back of the queue.
    // (In Aufgabe 2.1 you need INFINITY anyway, as the initial path cost
    // g(v) of every vertex except the start.)
    open.insert(3, INFINITY);   // espresso machine: no deadline known yet
    open.change_key(3, 1.0f); 

    // ------------------------------------------------------------------
    // 5. Draining the queue: del_min() returns the INDEX
    // ------------------------------------------------------------------
    // A very common mistake is to treat the return value of del_min() as the
    // priority. It is the index — the priority must be read BEFORE removal
    // (min_key()), because afterwards the entry is gone.
    std::cout << "processing order:" << std::endl;
    while (!open.is_empty()) {
        float hours_left = open.min_key();
        int i = open.del_min();
        std::cout << "  job " << i << " (" << job[i] << ", "
                  << hours_left << "h left)" << std::endl;
    }
    // expected order: espresso machine (1h), toaster (2h), laptop (3h), bike (8h)
    // The drone (index 4) was never inserted — the queue does not care.

    return 0;
}

// Things to try / predict before running:
//  (a) What does open.insert(7, 1.0f) do? (Hint: capacity is 5, and the class
//      does not check the index — look at contains() in vlib/index_min_pq.h.)
//  (b) What does open.change_key(4, 1.0f) throw, given that index 4 was never
//      inserted?
//  (c) In step 5, swap the two lines so that del_min() runs first and
//      min_key() second. Why is the output wrong (or an exception)?
//  (d) For A*: the priority stored here is f(v) = g(v) + h(v). Why do you
//      still need a separate map holding the pure path costs g(v)?
