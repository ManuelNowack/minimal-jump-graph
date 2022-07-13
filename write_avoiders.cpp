#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "pattern_avoidance.h"
using namespace std;
using namespace pattern_avoidance;


void write_avoiders(const vector<string>& patternlist, const int n_min, const int n_max, const string path)
{
    for (const string& pattern : patternlist)
    {
        const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted(pattern, n_max);
        for (int i = n_min; i <= n_max; i++)
        {
            const string out_file = path + "S_" + to_string(i) + "(" + pattern + ").txt";
            ofstream file(out_file);
            for (const perm_t& perm : avoiders[i])
            {
                file << perm_to_str(perm, i) << endl;
            }
        }
    }
}


int main()
{
    const vector<string> patterns = append_all_permutations({"1234", "1243"}, 4);
    const string path = "/home/manuel/Documents/Semester 8/Bachelor Thesis/code/avoiding patterns/";
    write_avoiders(patterns, 4, 10, path);
}
