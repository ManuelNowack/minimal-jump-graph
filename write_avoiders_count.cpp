#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "pattern_avoidance.h"
using namespace std;
using namespace pattern_avoidance;


void write_avoiders_count(const vector<string>& patterns, const int n_min, const int n_max, ostream& out)
{
    vector<vector<size_t>> avoiders_count(patterns.size(), vector<size_t>(n_max + 1));
    size_t max_count = 0;
    for (size_t i = 0; i < patterns.size(); i++)
    {
        vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(patterns[i], n_max);
        for (int j = n_min; j <= n_max; j++)
        {
            avoiders_count[i][j] = avoiders[j].size();
            if (max_count < avoiders_count[i][j])
            {
                max_count = avoiders_count[i][j];
            }
        }
    }

    const int padding_count = to_string(max_count).size();
    out << left << setw(10) << "F";
    for (int i = n_min; i <= n_max; i++)
    {
        out << setw(padding_count + 1) << i;
    }
    out << endl;
    for (size_t i = 0; i < patterns.size(); i++)
    {
        out << setw(10) << patterns[i];
        for (int j = n_min; j <= n_max; j++)
        {
            if (5000 < avoiders_count[i][j] && avoiders_count[i][j] <= 7500)
            {
                out << setw(padding_count + 1) << avoiders_count[i][j];
            }
            else
            {
                out << setw(padding_count + 1) << 0;
            }
        }
        out << endl;
    }
}


int main()
{
    const vector<string> patterns = append_all_permutations({"1234", "1324", "2134", "2314", "3124", "3214"}, 4);
    const string path = "/home/manuel/Documents/Semester 8/Bachelor Thesis/code/hamiltonicity overview/";
    ofstream out_file(path + "count.txt");
    write_avoiders_count(patterns, 4, 10, out_file);
}
