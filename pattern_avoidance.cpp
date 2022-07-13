#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "countavoiders.h"
#include "perm.h"
#include "pattern_avoidance.h"
using namespace std;


namespace pattern_avoidance {

int64_t factorial(const int n)
{
    assert(n <= 20);
    int64_t result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}


vector<string> append_all_permutations(const vector<string>& patterns, const int n)
{
    vector<string> new_patterns;
    new_patterns.reserve(patterns.size() * (size_t)(factorial(n)));
    const vector<string> all_permutations = compute_all_permutations_simple_string(n);
    for (const string& pattern : patterns)
    {
        new_patterns.push_back(pattern);
        for (const string& permutation : all_permutations)
        {
            if (pattern != permutation)
            {
                new_patterns.push_back(pattern + " " + permutation);
            }
        }
    }
    assert(new_patterns.capacity() == new_patterns.size());
    return new_patterns;
}


bool compare_perm(const perm_t lhs, const perm_t rhs, const int n)
{
    for (int i = 0; i < n; i++)
    {
        const uint64_t lhs_digit = getdigit(lhs, i);
        const uint64_t rhs_digit = getdigit(rhs, i);
        if (lhs_digit < rhs_digit)
        {
            return true;
        }
        else if (lhs_digit > rhs_digit)
        {
            return false;
        }
    }
    return false;
}


vector<perm_t> compute_all_permutations(const int n)
{
    assert(n <= 20);
    vector<uint64_t> digits(n);
    for (int i = 0; i < n; i++)
    {
        digits[i] = i;
    }
    vector<perm_t> all_permutations;
    all_permutations.reserve(factorial(n));
    do {
        perm_t perm = 0;
        for (int i = 0; i < n; i++)
        {
            perm = setdigit(perm, i, digits[i]);
        }
        all_permutations.push_back(perm);
    }
    while (next_permutation(digits.begin(), digits.end()));
    return all_permutations;
}


vector<string> compute_all_permutations_simple_string(const int n)
{
    assert(n < 10);
    string permutation = perm_to_str_simple(identity_permutation(n), n);
    vector<string> all_permutations;
    all_permutations.reserve(factorial(n));
    do {
        all_permutations.push_back(permutation);
    }
    while (next_permutation(permutation.begin(), permutation.end()));
    return all_permutations;
}


vector<int> get_ascents(const perm_t perm, const int n)
{
    vector<int> ascents;
    uint64_t curr_digit = getdigit(perm, 0);
    for (int i = 0; i < n - 1; i++)
    {
        uint64_t next_digit = getdigit(perm, i + 1);
        if (curr_digit < next_digit)
        {
            ascents.push_back(i);
        }
        curr_digit = next_digit;
    }
    ascents.shrink_to_fit();
    assert(ascents.capacity() == ascents.size());
    return ascents;
}


int get_digit_position(const perm_t perm, const uint64_t digit)
{
    for (int i = 0; i < MAXPERMSIZE; i++)
    {
        if (getdigit(perm, i) == digit)
        {
            return i;
        }
    }
    return -1;
}


int get_num_ascents(const perm_t perm, const int n)
{
    int ascents = 0;
    uint64_t prev_digit = getdigit(perm, 0);
    for (int i = 1; i < n; i++)
    {
        uint64_t curr_digit = getdigit(perm, i);
        if (prev_digit < curr_digit)
        {
            ascents += 1;
        }
        prev_digit = curr_digit;
    }
    return ascents;
}


int inversion_number(const perm_t perm, const int n)
{
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (getdigit(perm, i) > getdigit(perm, j))
            {
                result += 1;
            }
        }
    }
    return result;
}


perm_t identity_permutation(const int n)
{
    perm_t perm = 0;
    for (int i = 0; i < n; i++)
    {
        perm = setdigit(perm, i, i);
    }
    return perm;
}


vector<vector<perm_t>> pattern_avoiding_permutations(const string pattern, const int n)
{
    vector<list<perm_t>> avoiders_list;
    buildavoidersfrompatternlist(pattern, n, avoiders_list);
    vector<vector<perm_t>> avoiders(n + 1);
    for (int i = 1; i <= n; i++)
    {
        avoiders[i].reserve(avoiders_list[i].size());
        copy(begin(avoiders_list[i]), end(avoiders_list[i]), back_inserter(avoiders[i]));
    }
    return avoiders;
}


vector<vector<perm_t>> pattern_avoiding_permutations_sorted(const string pattern, const int n)
{
    vector<vector<perm_t>> avoiders = pattern_avoiding_permutations(pattern, n);
    for (int i = 1; i <= n; i++)
    {
        sort(avoiders[i].begin(), avoiders[i].end(), [i](perm_t lhs, perm_t rhs) { return compare_perm(lhs, rhs, i); });
    }
    return avoiders;
}

string perm_to_str(const perm_t perm, const int n)
{
    ostringstream buffer;
    for (int i = 0; i < n - 1; i++)
    {
        buffer << getdigit(perm, i) + 1 << " ";
    }
    buffer << getdigit(perm, n - 1) + 1;
    return buffer.str();
}


string perm_to_str_simple(const perm_t perm, const int n)
{
    ostringstream buffer;
    for (int i = 0; i < n - 1; i++)
    {
        buffer << getdigit(perm, i) + 1;
    }
    buffer << getdigit(perm, n - 1) + 1;
    return buffer.str();
}


perm_t string_to_permutation(const string permutation)
{
    perm_t perm = 0;
    for (size_t i = 0; i < permutation.size(); i++)
    {
        // Convert ASCII character to number and subtract 1 because digits in perm_t are zero-based
        const char digit = permutation[i] - 49;
        perm = setdigit(perm, i, digit);
    }
    return perm;
}

} // namespace pattern_avoidance
