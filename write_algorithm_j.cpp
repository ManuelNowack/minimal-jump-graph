#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "pattern_avoidance.h"
#include "minimal_jump_graph.h"
using namespace std;
using namespace minimal_jump;
using namespace pattern_avoidance;


vector<perm_t> algorithm_j(const vector<perm_t>& S, const int n, const perm_t start)
{
    vector<perm_t> sequence;
    sequence.reserve(S.size());
    const unordered_set<perm_t> S_hashtable(S.begin(), S.end());
    unordered_map<perm_t, bool> visited(S.size());
    visited[start] = true;
    sequence.push_back(start);
    perm_t most_recent = start;
    bool terminate = false;
    // bool pick_right = true;
    while (!terminate)
    {
        vector<min_jump_t> min_jumps = compute_min_jumps_verbose(most_recent, n, S_hashtable);
        sort(min_jumps.begin(), min_jumps.end(), [](min_jump_t lhs, min_jump_t rhs) { return lhs.digit > rhs.digit; });
        terminate = true;
        for (size_t i = 0; i < min_jumps.size(); i++)
        {
            if (!visited[min_jumps[i].to])
            {
                if (i + 1 < min_jumps.size() && min_jumps[i].digit == min_jumps[i + 1].digit && !visited[min_jumps[i + 1].to])
                {
                    // jump direction is ambiguous
                    // cout << "terminating at " << perm_to_str(min_jumps[i].to, n) << " because direction is ambiguous" << endl;
                    break;
                    // resolve by picking alternating right or left jump
                    // if ((pick_right && min_jumps[i].right) || (!pick_right && !min_jumps[i].right))
                    // {
                    //     most_recent = min_jumps[i].to;
                    // }
                    // else
                    // {
                    //     most_recent = min_jumps[i].to;
                    // }
                    // pick_right = !pick_right;
                    // resolve by picking both and violate minimal jump property
                    // sequence.push_back(min_jumps[i].to);
                    // visited[min_jumps[i].to] = true;
                    // most_recent = min_jumps[i + 1].to;
                }
                // cout << "selecting " << perm_to_str(min_jumps[i].to, n) << endl;
                most_recent = min_jumps[i].to;
                sequence.push_back(most_recent);
                visited[most_recent] = true;
                terminate = false;
                break;
            }
        }
    }
    return sequence;
}


vector<perm_t> algorithm_j(const vector<perm_t>& S, const int n)
{
    if (S.empty())
    {
        return vector<perm_t>();
    }
    return algorithm_j(S, n, S.front());
}


int main()
{
    vector<string> patternlist = {"1234",
                                  "1324",
                                  "2134",
                                  "2314",
                                  "3124",
                                  "3214",
                                  "4123",
                                  "4132",
                                  "4213",
                                  "4231",
                                  "4312",
                                  "4321"};
    patternlist = append_all_permutations(patternlist, 4);
    constexpr int n_min = 4;
    constexpr int n_max = 8;
    for (const string& pattern : patternlist)
    {
        const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(pattern, n_max);
        bool success = true;
        for (int i = n_min; i <= n_max; i++)
        {
            for (const perm_t& perm : avoiders[i])
            {
                const vector<perm_t> sequence = algorithm_j(avoiders[i], i, perm);
                if (sequence.size() != avoiders[i].size())
                {
                    // cout << "Algorithm J failed for S_" << i << "(" << pattern << "): Expected " << avoiders[i].size() << " but got " << sequence.size() << endl;
                    success = false;
                }
                else
                {
                    cout << "Algorithm J succeeded for S_" << i << "(" << pattern << ") with " << perm_to_str(perm, i) << endl;
                }
            }
        }
        if (success)
        {
            cout << "Algorithm J succeeded for up to S_" << n_max << "(" << pattern << ")" << endl;
        }
    }
    return 0;
}
