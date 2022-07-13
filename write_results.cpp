#include <algorithm>
#include <cstdio>     /* fgets, popen, pclose */
#include <cstdlib>    /* mkdtemp */
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "minimal_jump_graph.h"
#include "pattern_avoidance.h"
using namespace std;
using namespace minimal_jump;
using namespace pattern_avoidance;


constexpr char CONCORDE[] = "/etc/concorde/TSP/concorde";
constexpr int CONCORDE_MAX_INPUT_SIZE = 7000;


bool run_process(const char* cmd)
{
    FILE *process = popen(cmd, "r");
    char buff[128];
    while (fgets(buff, 128, process))
    {
        cout << buff;
    }
    if (!process || pclose(process) < 0)
    {
        return false;
    }
    return true;
}


/** Returns
     1 if the graph has a Hamiltonian cycle
     0 if the graph has no Hamiltonian cycle
    -1 if temporary directory for concorde files could not be created
    -2 if .tsp file for concorde could not be created
    -3 if running concorde fails
    -4 if .sol file from concorde could not be read
*/
template<typename T>
int get_hamiltonian_cycle(const vector<vector<T>>& G, const string pattern, const int n, vector<T>& solution)
{
    char temp_dir_template[] = "/tmp/XXXXXX";
    if (!mkdtemp(temp_dir_template))
    {
        return -1;
    }
    const string temp_dir = temp_dir_template;
    const string tsp_file = temp_dir + "/a.tsp";
    const string sol_file = temp_dir + "/a.sol";
    const string concorde(CONCORDE);
    const string cmd = "cd " + temp_dir + " && " + concorde + " a.tsp";
    if (!write_tsp_file(G, pattern, n, false, tsp_file))
    {
        return -2;
    }
    if (!run_process(cmd.c_str()))
    {
        return -3;
    }
    if (!read_sol_file(sol_file, solution))
    {
        return -4;
    }
    return has_cycle(G, solution);
}


/** Returns
     1 if the graph has a Hamiltonian path
     0 if the graph has no Hamiltonian path
    -1 if temporary directory for concorde files could not be created
    -2 if .tsp file for concorde could not be created
    -3 if running concorde fails
    -4 if .sol file from concorde could not be read
*/
template<typename T>
int get_hamiltonian_path(const vector<vector<T>>& G, const string pattern, const int n, vector<T>& solution)
{
    char temp_dir_template[] = "/tmp/XXXXXX";
    if (!mkdtemp(temp_dir_template))
    {
        return -1;
    }
    const string temp_dir = temp_dir_template;
    const string tsp_file = temp_dir + "/a.tsp";
    const string sol_file = temp_dir + "/a.sol";
    const string concorde(CONCORDE);
    const string cmd = "cd " + temp_dir + " && " + concorde + " a.tsp";
    if (!write_tsp_file(G, pattern, n, true, tsp_file))
    {
        return -2;
    }
    if (!run_process(cmd.c_str()))
    {
        return -3;
    }
    if (!read_sol_file(sol_file, solution))
    {
        return -4;
    }
    const typename vector<T>::iterator aux = find(solution.begin(), solution.end(), solution.size() - 1);
    reverse(solution.begin(), aux);
    reverse(next(aux), solution.end());
    solution.erase(aux);
    return has_path(G, solution);
}


/** Returns
    1 if the graph has a Hamiltonian cycle
    0 if the graph has no Hamiltonian cycle
    -1 if programm was compiled without concorde
    -2 if temporary directory for concorde files could not be created
    -3 if .tsp file for concorde could not be created
    -4 if running concorde fails
    -5 if .sol file from concorde could not be read
*/
template<typename T>
int check_Hamiltonian_cycle(const vector<vector<T>>& G, const string pattern, const int n)
{
    vector<T> solution;
    return get_hamiltonian_cycle(G, pattern, n, solution);
}


/** Returns
     1 if the graph has a Hamiltonian path
     0 if the graph has no Hamiltonian path
    -1 if programm was compiled without concorde
    -2 if temporary directory for concorde files could not be created
    -3 if .tsp file for concorde could not be created
    -4 if running concorde fails
    -5 if .sol file from concorde could not be read
*/
template<typename T>
int check_Hamiltonian_path(const vector<vector<T>>& G, const string pattern, const int n)
{
    vector<T> solution;
    return get_hamiltonian_path(G, pattern, n, solution);
}


void write_results(const vector<string>& patternlist, const int n_min, const int n_max, ostream& out)
{
    const int pattern_padding = max_element(patternlist.begin(), patternlist.end(), [](string lhs, string rhs) { return lhs.size() < rhs.size(); })->size() + 1;
    out << left << setw(pattern_padding) << "F";
    for (int i = n_min; i <= n_max; i++)
    {
        out << setw(4) << i;
    }
    out << endl;

    for (const string& pattern : patternlist)
    {
        out << setw(pattern_padding) << pattern;
        vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(pattern, n_max);
        for (int i = n_min; i <= n_max; i++)
        {
            cout << "building graph of " << avoiders[i].size() << " nodes for " << pattern << " with n=" << i << endl;
            const vector<vector<int64_t>> G = compute_adjacency_list<int64_t>(avoiders[i], i);
            const bool empty = minimal_jump::is_empty(G);
            const bool disconnected = is_disconnected(G);
            const int64_t degree_one_vertices_count = get_count_of_degree(G, (int64_t) 1);
            // bipartition is undefined for disconnected graphs
            const int64_t bipartition_size_difference = (disconnected) ? 0 : get_bipartition_size_difference(G);
            if (bipartition_size_difference > 1 && degree_one_vertices_count > 2)
            {
                out << setw(4) << "Xbd";
            }
            else if (disconnected && degree_one_vertices_count > 2)
            {
                out << setw(4) << "Xcd";
            }
            else if (bipartition_size_difference > 1)
            {
                out << setw(4) << "Xb";
            }
            else if (disconnected)
            {
                out << setw(4) << "Xc";
            }
            else if (degree_one_vertices_count > 2)
            {
                out << setw(4) << "Xd";
            }
            else if (empty)
            {
                out << setw(4) << "Xe";
            }
            else
            {
                if (G.size() > CONCORDE_MAX_INPUT_SIZE)
                {
                    out << setw(4) << "?";
                }
                else
                {
                    const int cycle = check_Hamiltonian_cycle(G, pattern, i);
                    const int path = (cycle == 1) ? 1 : check_Hamiltonian_path(G, pattern, i);
                    if (cycle < 0 || path < 0)
                    {
                        cerr << "cycle return code: " << cycle << ", path return code: " << path << endl; 
                        out << setw(4) << "FAIL";
                    }
                    else if (cycle == 1)
                    {
                        out << setw(4) << "C";
                    }
                    else if (path == 1)
                    {
                        if (degree_one_vertices_count > 0 && bipartition_size_difference > 0)
                        {
                            out << setw(4) << "Pbd";
                        }
                        else if (bipartition_size_difference > 0)
                        {
                            out << setw(4) << "Pb";
                        }
                        else if (degree_one_vertices_count > 0)
                        {
                            out << setw(4) << "Pd";
                        }
                        else
                        {
                            vector<vector<int64_t>> G_optimize = G;
                            while (prune_degree_two_vertices(G_optimize));
                            const bool pruned_disconnected = is_disconnected(G_optimize);
                            const int64_t pruned_degree_one_vertices_count = get_count_of_degree(G_optimize, (int64_t) 1);
                            // bipartition is undefined for disconnected graphs
                            const int64_t pruned_bipartition_size_difference = (disconnected) ? 0 : get_bipartition_size_difference(G_optimize);
                            if (pruned_bipartition_size_difference > 0 && pruned_degree_one_vertices_count > 0)
                            {
                                out << setw(4) << "P2bd";
                            }
                            else if (pruned_disconnected && pruned_degree_one_vertices_count > 0)
                            {
                                out << setw(4) << "P2cd";
                            }
                            else if (pruned_bipartition_size_difference > 0)
                            {
                                out << setw(4) << "P2b";
                            }
                            else if (pruned_disconnected)
                            {
                                out << setw(4) << "P2c";
                            }
                            else if (pruned_degree_one_vertices_count > 0)
                            {
                                out << setw(4) << "P2d";
                            }
                            else
                            {
                                out << setw(4) << "P?";
                            }
                        }
                    }
                    else
                    {
                        out << setw(4) << "X?";
                    }
                }
            }
        }
        out << endl;
    }
}


int main()
{
    const vector<string> patterns = append_all_permutations({"1234", "1243", "2134", "2314", "3124", "3214"}, 4);
    ofstream out_file("hamiltonicity.txt");
    write_results(patterns, 4, 12, out_file);
    return 0;
}
