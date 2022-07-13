#ifndef _MINIMAL_JUMP_GRAPH_H 
#define _MINIMAL_JUMP_GRAPH_H

#include <cstdint>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "pattern_avoidance.h"
using namespace std;  // pattern_avoidance.h already enforces namespace std...


namespace minimal_jump {

typedef struct min_jump_t {
    perm_t from;
    perm_t to;
    bool right;
    int steps;
    uint64_t digit;
    min_jump_t(perm_t from, perm_t to, bool right, int steps, uint64_t digit)
        : from(from)
        , to(to)
        , right(right)
        , steps(steps)
        , digit(digit)
    { }
} min_jump_t;


/** Returns all minimal jumps for a permutation of length n with respect to a
    set of permutations.
*/
vector<perm_t> compute_min_jumps(const perm_t perm, const int n, const unordered_set<perm_t>& S);


/** Returns all minimal jumps for a permutation of length n with respect to a
    set of permutations. The minimal jumps are returned as min_jump_t structs.
*/
vector<min_jump_t> compute_min_jumps_verbose(const perm_t perm, const int n, const unordered_set<perm_t>& S);


/** Returns the minimal jump graph for permutations of length n.

    The minimal jump graph is stored as an adjacency list. The vertex labels
    are given by perm_to_vertex_mapping<T>(S). The only supported typename is
    int64_t.
*/
template<typename T>
vector<vector<T>> compute_adjacency_list(const vector<perm_t>& S, const int n);


/** Returns the size difference of the bipartition of a graph. If the graph
    is not bipartite, 0 is returned. Therefore this function cannot distinguish
    a graph with balanced bipartition from a graph with no bipartition. However
    this distinction is not needed to decide Hamiltonicity of a graph.
*/
template<typename T>
T get_bipartition_size_difference(const vector<vector<T>>& G);


/** Returns the number of vertices in a graph that have a particular degree.
*/
template<typename T>
T get_count_of_degree(const vector<vector<T>>& G, const T degree);


/** Returns the vertices of a graph that have a particular degree.
*/
template<typename T>
vector<T> get_vertices_of_degree(const vector<vector<T>>& G, const T degree);


/** Returns true if a graph contains a particular cycle. The cycle is given as
    a vector of the visited vertices. The final vertex is omitted.
*/
template<typename T>
bool has_cycle(const vector<vector<T>>& G, const vector<T>& cycle);


/** Returns true if a graph contains a particular path. The path is given as
    a vector of the visited vertices.
*/
template<typename T>
bool has_path(const vector<vector<T>>& G, const vector<T>& path);


/** Returns true if and only if a graph has disconnected components.
*/
template<typename T>
bool is_disconnected(const vector<vector<T>>& G);


/** Returns true if and only if a graph is empty, i.e. has no vertices or
    edges.
*/
template<typename T>
bool is_empty(const vector<vector<T>>& G);


/** Returns a mapping from a vector S of permutations to [0, 1, ..., |S|-1]
    corresponding to the order of the permutations in S.

    Asserts that the arithmetic type T can store values up to |S|.
*/
template<typename T>
unordered_map<perm_t, T> perm_to_vertex_mapping(const vector<perm_t>& S);


/** Deletes edges that are adjacent to a vertex that has two degree two
    neighbours. Such edges cannot be part of a Hamiltonian cycle.

    Returns true if and only if edges have been removed.
*/
template<typename T>
bool prune_degree_two_vertices(vector<vector<T>>& G);


/** Reads a .sol file computed by the Concorde TSP solver and stores the cycle
    in the parameter solution.

    Returns true if and only the .sol file was successfully read.
*/
template<typename T>
bool read_sol_file(const string in_file, vector<T>& solution);


/** Writes a file in the TSPLIB format containing a TSP problem equivalent to 
    the Hamiltonian cycle/path problem (determined by parameter path_problem)
    of a minimal jump graph avoiding a pattern of length n.

    Returns true if and only if the file was successfully written at the path
    given by parameter out_file.
*/
template<typename T>
bool write_tsp_file(const vector<vector<T>>& G, const string pattern, const int n, const bool path_problem, const string out_file);

} // namespace minimal_jump

#endif
