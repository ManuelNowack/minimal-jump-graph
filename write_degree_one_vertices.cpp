#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "minimal_jump_graph.h"
#include "pattern_avoidance.h"
using namespace std;
using namespace minimal_jump;
using namespace pattern_avoidance;


void write_degree_one_vertices(const string pattern, const int n_min, const int n_max, ostream& out)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(pattern, n_max);
    for (int i = n_min; i <= n_max; i++)
    {
        const vector<vector<int64_t>> G = compute_adjacency_list<int64_t>(avoiders[i], i);
        const vector<int64_t> degree_one_vertices = get_vertices_of_degree(G, (int64_t) 1);
        if (degree_one_vertices.size() > 0)
        {
            vector<perm_t> degree_one_permutations;
            degree_one_permutations.reserve(degree_one_vertices.size());
            for (const int64_t& vertex : degree_one_vertices)
            {
                degree_one_permutations.push_back(avoiders[i][vertex]);
            }
            sort(degree_one_permutations.begin(), degree_one_permutations.end(), [i](perm_t lhs, perm_t rhs) { return compare_perm(lhs, rhs, i); });
            out << pattern << "  n=" << i << endl << endl;
            for (const perm_t& perm : degree_one_permutations)
            {
                out << perm_to_str(perm, i) << endl;
            }
            if (i < n_max)
            {
                out << endl << endl;
            }
        }
    }
}


void write_degree_one_vertices_count(const vector<string>& patternlist, const int n_min, const int n_max, ostream& out)
{
    vector<vector<int64_t>> degree_one_vertices_count(patternlist.size(), vector<int64_t>(n_max + 1));
    int64_t max_count = 0;
    for (size_t i = 0; i < patternlist.size(); i++)
    {
        const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(patternlist[i], n_max);
        for (int j = n_min; j <= n_max; j++)
        {
            const vector<vector<int64_t>> G = compute_adjacency_list<int64_t>(avoiders[j], j);
            const int64_t count = get_count_of_degree(G, (int64_t) 1);
            degree_one_vertices_count[i][j] = count;
            if (count > max_count)
            {
                max_count = count;
            }
        }
    }
    const int padding_formula = max_element(patternlist.begin(), patternlist.end(), [](string lhs, string rhs) { return lhs.size() < rhs.size(); })->size();
    const int padding_degree = to_string(max_count).size();
    out << left << setw(padding_formula + 1) << "F";
    for (int i = n_min; i <= n_max; i++)
    {
        out << setw(padding_degree + 1) << i;
    }
    out << endl;
    for (size_t i = 0; i < patternlist.size(); i++)
    {
        out << setw(padding_formula + 1) << patternlist[i];
        for (int j = n_min; j <= n_max; j++)
        {
            out << setw(padding_degree + 1) << degree_one_vertices_count[i][j];
        }
        out << endl;
    }
    out << endl;
}


int main()
{
    const vector<string> patterns = append_all_permutations({"123", "213"}, 3);
    // const vector<string> patterns = append_all_permutations({"1234", "1243", "2134", "2314", "3124", "3214"}, 4);
    for (const string& pattern : patterns)
    {
        ofstream file_vertices(pattern + ".txt");
        write_degree_one_vertices(pattern, 4, 10, file_vertices);
    }
    return 0;
}
