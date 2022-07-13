#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "pattern_avoidance.h"
#include "minimal_jump_graph.h"
using namespace std;
using namespace minimal_jump;
using namespace pattern_avoidance;


void write_min_jumps(const string pattern, const int n_min, const int n_max, const string path)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted(pattern, n_max);
    for (int i = n_min; i <= n_max; i++)
    {
        const unordered_set<perm_t> avoiders_hashtable(avoiders[i].begin(), avoiders[i].end());
        const string out_file = path + "G_" + to_string(i) + "(" + pattern + ").txt";
        ofstream file(out_file);
        for (const perm_t& perm : avoiders[i])
        {
            vector<perm_t> min_jumps = compute_min_jumps(perm, i, avoiders_hashtable);
            sort(min_jumps.begin(), min_jumps.end(), [i](perm_t lhs, perm_t rhs) { return compare_perm(lhs, rhs, i); });
            file << perm_to_str(perm, i) << endl << endl;
            for (const perm_t& min_jump : min_jumps)
            {
                file << "\t" << perm_to_str(min_jump, i) << endl;
            }
            file << endl << endl;
        }
    }
}


int main()
{
    const vector<string> patterns = append_all_permutations({"2134", "2314", "3124", "3214"}, 4);
    const string path = "/home/manuel/Documents/Semester 8/Bachelor Thesis/code/minimal jump graphs/";
    for (const string& pattern : patterns)
    {
        write_min_jumps(pattern, 4, 8, path);
    }
}
