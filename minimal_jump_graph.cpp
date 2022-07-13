#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <list>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "minimal_jump_graph.h"
#include "pattern_avoidance.h"
using namespace std;
using namespace pattern_avoidance;


namespace minimal_jump {

vector<perm_t> compute_min_jumps(const perm_t perm, const int n, const unordered_set<perm_t>& S)
{
    vector<perm_t> min_jumps;
    for (int i = 0; i < n; i++)
    {
        const uint64_t entry_i = getdigit(perm, i);
        perm_t right_jump = perm;
        for (int j = i + 1; j < n; j++)
        {
            const uint64_t entry_j = getdigit(perm, j);
            if (entry_i < entry_j)
            {
                break;
            }
            right_jump = setdigit(right_jump, j, entry_i);
            right_jump = setdigit(right_jump, j - 1, entry_j);
            if (S.count(right_jump) == 1)
            {
                min_jumps.push_back(right_jump);
                break;
            }
        }
        perm_t left_jump = perm;
        for (int j = i - 1; j >= 0; --j)
        {
            const uint64_t entry_j = getdigit(perm, j);
            if (entry_i < entry_j)
            {
                break;
            }
            left_jump = setdigit(left_jump, j, entry_i);
            left_jump = setdigit(left_jump, j + 1, entry_j);
            if (S.count(left_jump) == 1)
            {
                min_jumps.push_back(left_jump);
                break;
            }
        }
    }
    return min_jumps;
}


vector<min_jump_t> compute_min_jumps_verbose(const perm_t perm, const int n, const unordered_set<perm_t>& S)
{
    vector<min_jump_t> min_jumps;
    for (int i = 0; i < n; i++)
    {
        const uint64_t entry_i = getdigit(perm, i);
        perm_t right_jump = perm;
        for (int j = i + 1; j < n; j++)
        {
            const uint64_t entry_j = getdigit(perm, j);
            if (entry_i < entry_j)
            {
                break;
            }
            right_jump = setdigit(right_jump, j, entry_i);
            right_jump = setdigit(right_jump, j - 1, entry_j);
            if (S.count(right_jump) == 1)
            {
                min_jumps.push_back(min_jump_t(perm, right_jump, true, j - i, entry_i));
                break;
            }
        }
        perm_t left_jump = perm;
        for (int j = i - 1; j >= 0; --j)
        {
            const uint64_t entry_j = getdigit(perm, j);
            if (entry_i < entry_j)
            {
                break;
            }
            left_jump = setdigit(left_jump, j, entry_i);
            left_jump = setdigit(left_jump, j + 1, entry_j);
            if (S.count(left_jump) == 1)
            {
                min_jumps.push_back(min_jump_t(perm, left_jump, false, i - j, entry_i));
                break;
            }
        }
    }
    return min_jumps;
}


template<typename T>
vector<vector<T>> compute_adjacency_list(const vector<perm_t>& S, const int n)
{
    const unordered_map<perm_t, T> vertex_map = perm_to_vertex_mapping<T>(S);
    const unordered_set<perm_t> S_hashtable(S.begin(), S.end());
    vector<vector<T>> G(S.size());
    for (const perm_t& perm : S)
    {
        const T u = vertex_map.at(perm);
        vector<perm_t> min_jumps = compute_min_jumps(perm, n, S_hashtable);
        G[u].reserve(min_jumps.size());
        for (const perm_t& jump : min_jumps)
        {
            const T v = vertex_map.at(jump);
            G[u].push_back(v);
        }
    }
    return G;
}


template<typename T>
T get_bipartition_size_difference(const vector<vector<T>>& G)
{
    if (G.size() < 2)
    {
        return false;
    }
    vector<bool> blue(G.size(), false);
    vector<bool> red(G.size(), false);

    list<T> curr_round, next_round;
    curr_round.push_back(G.front().front());
    bool is_blue_round = true;
    while (!curr_round.empty())
    {
        for (const T& curr_node : curr_round)
        {
            if (is_blue_round)
            {
                if (red[curr_node])
                {
                    return 0;
                }
                if (!blue[curr_node])
                {
                    blue[curr_node] = true;
                    for (const T& new_node : G[curr_node])
                    {
                        next_round.push_back(new_node);
                    }
                }
            }
            else
            {
                if (blue[curr_node])
                {
                    return 0;
                }
                if (!red[curr_node])
                {
                    red[curr_node] = true;
                    for (const T& new_node : G[curr_node])
                    {
                        next_round.push_back(new_node);
                    }
                }
            }
        }
        curr_round.clear();
        swap(curr_round, next_round);
        is_blue_round = !is_blue_round;
    }
    const T count_blue = count(blue.begin(), blue.end(), true);
    const T count_red = count(red.begin(), red.end(), true);
    if (count_blue > count_red)
    {
        return count_blue - count_red;
    }
    else
    {
        return count_red - count_blue;
    }
}


template<typename T>
T get_count_of_degree(const vector<vector<T>>& G, const T degree)
{
    T count = 0;
    for (const vector<T>& adj_list : G)
    {
        if ((T) adj_list.size() == degree)
        {
            count += 1;
        }
    }
    return count;
}


template<typename T>
vector<T> get_vertices_of_degree(const vector<vector<T>>& G, const T degree)
{
    vector<T> vertices;
    for (size_t i = 0; i < G.size(); i++)
    {
        if ((T) G[i].size() == degree)
        {
            vertices.push_back((T) i);
        }
    }
    return vertices;
}


template<typename T>
bool has_cycle(const vector<vector<T>>& G, const vector<T>& cycle)
{
    for (size_t i = 0; i < cycle.size(); i++)
    {
        const T u = cycle[i];
        const T v = (i == cycle.size() - 1) ? cycle[0] : cycle[i + 1];
        const bool edge_exists = find(G[u].begin(), G[u].end(), v) != G[u].end();
        if (!edge_exists)
        {
            return false;
        }
    }
    return true;
}


template<typename T>
bool has_path(const vector<vector<T>>& G, const vector<T>& path)
{
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        const T u = path[i];
        const T v = path[i + 1];
        const bool edge_exists = find(G[u].begin(), G[u].end(), v) != G[u].end();
        if (!edge_exists)
        {
            return false;
        }
    }
    return true;
}


template<typename T>
bool is_disconnected(const vector<vector<T>>& G)
{
    vector<bool> visited(G.size(), false);
    list<T> q;
    for (const vector<T>& adj_list : G)
    {
        if (!adj_list.empty())
        {
            const T start = adj_list.front();
            q.push_back(start);
            visited[start] = true;
            break;
        }
    }
    while (!q.empty())
    {
        const T curr_node = q.front();
        q.pop_front();
        for (const T& new_node : G[curr_node])
        {
            if (!visited[new_node])
            {
                q.push_back(new_node);
                visited[new_node] = true;
            }
        }
    }
    const T count_visited = count(visited.begin(), visited.end(), true);
    return count_visited != (T) G.size();
}


template<typename T>
bool is_empty(const vector<vector<T>>& G)
{
    return G.size() == 0;
}


template<typename T>
unordered_map<perm_t, T> perm_to_vertex_mapping(const vector<perm_t>& S)
{
    // the elements of S cannot be mapped to type T if |S| is larger than the maximum value of T
    assert(S.size() < numeric_limits<T>::max());
    unordered_map<perm_t, T> vertex_map(S.size());
    T curr_vertex = 0;
    for (const perm_t& perm : S)
    {
        vertex_map[perm] = curr_vertex += 1;
    }
    return vertex_map;
}


template<typename T>
bool prune_degree_two_vertices(vector<vector<T>>& G)
{
    bool optimization_occured = false;
    const vector<T> degree_two_vertices = get_vertices_of_degree(G, (T) 2);
    for (const T& vertex_1 : degree_two_vertices)
    {
        for (const T& neighbor : G[vertex_1])
        {
            for (const T& vertex_2 : G[neighbor])
            {
                if (vertex_1 != vertex_2 && G[neighbor].size() > 2 && find(degree_two_vertices.begin(), degree_two_vertices.end(), vertex_2) != degree_two_vertices.end())
                {
                    for (const T& neighbor_to : G[neighbor])
                    {
                        if (neighbor_to != vertex_1 && neighbor_to != vertex_2)
                        {
                            const typename vector<T>::iterator it = find(G[neighbor_to].begin(), G[neighbor_to].end(), neighbor);
                            if (it != G[neighbor_to].end())
                            {
                                G[neighbor_to].erase(it);
                            }
                        }
                    }
                    G[neighbor] = {vertex_1, vertex_2};
                    optimization_occured = true;
                }
            }
        }
    }
    return optimization_occured;
}


template<typename T>
bool read_sol_file(const string in_file, vector<T>& solution)
{
    ifstream file(in_file);
    if (!file.good())
    {
        return false;
    }
    size_t num_vertices;
    file >> num_vertices;
    solution.resize(num_vertices);
    for (size_t i = 0; i < num_vertices; i++)
    {
        file >> solution[i];
    }
    return true;
}


template<typename T>
bool write_tsp_file(const vector<vector<T>>& G, const string pattern, const int n, const bool path_problem, const string out_file)
{
    ofstream file(out_file);
    if (!file.good())
    {
        return false;
    }
    file << "NAME : G_" << n << "(" << pattern << ")" << endl;
    file << "TYPE : TSP" << endl;
    if (path_problem)
    {
        file << "DIMENSION : " << G.size() + 1 << endl;
        file << "COMMENT : Hamiltonian Path Problem" << endl;
    }
    else
    {
        file << "DIMENSION : " << G.size() << endl;
        file << "COMMENT : Hamiltonian Cycle Problem" << endl;
    }
    file << "EDGE_WEIGHT_TYPE: EXPLICIT" << endl;
    file << "EDGE_WEIGHT_FORMAT: UPPER_ROW" << endl;
    file << "EDGE_WEIGHT_SECTION" << endl;
    // set edge weight to zero if the edge exists in the graph, otherwise
    // set edge weight to one.
    // if hamiltonian path we add an auxiliary vertex that is connected to
    // all other vertices with weight zero
    for (size_t i = 0; i < G.size(); i++)
    {
        vector<bool> adj_vec(G.size(), false);
        for (const T& vertex : G[i])
        {
            adj_vec[vertex] = true;
        }
        for (size_t j = 0; j < G.size(); j++)
        {
            if (i < j)
            {
                if (adj_vec[j])
                {
                    file << " 0";
                }
                else
                {
                    file << " 1";
                }
            }
            else
            {
                file << "  ";
            }
        }
        if (path_problem)
        {
            file << " 0";
        }
        file << endl;
    }
    file << "EOF" << endl;
    return true;
}


template vector<vector<int64_t>> compute_adjacency_list(const vector<perm_t>& S, const int n);

template int64_t get_bipartition_size_difference(const vector<vector<int64_t>>& G);

template int64_t get_count_of_degree(const vector<vector<int64_t>>& G, const int64_t degree);

template vector<int64_t> get_vertices_of_degree(const vector<vector<int64_t>>& G, const int64_t degree);

template bool has_cycle(const vector<vector<int64_t>>& G, const vector<int64_t>& cycle);

template bool has_path(const vector<vector<int64_t>>& G, const vector<int64_t>& path);

template bool is_disconnected(const vector<vector<int64_t>>& G);

template bool is_empty(const vector<vector<int64_t>>& G);

template unordered_map<perm_t, int64_t> perm_to_vertex_mapping(const vector<perm_t>& S);

template bool prune_degree_two_vertices(vector<vector<int64_t>>& G);

template bool read_sol_file(const string in_file, vector<int64_t>& solution);

template bool write_tsp_file(const vector<vector<int64_t>>& G, const string pattern, const int n, const bool path_problem, const string out_file);

}
