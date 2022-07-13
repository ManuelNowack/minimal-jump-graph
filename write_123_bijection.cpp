#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "pattern_avoidance.h"
#include "minimal_jump_graph.h"
using namespace std;
using namespace minimal_jump;
using namespace pattern_avoidance;


int get_first_ascent(const perm_t perm, const int n)
{
    const vector<int> ascents = get_ascents(perm, n);
    if (ascents.empty())
    {
        return n - 1;
    }
    return ascents[0];
}


int64_t max_vector2d(vector<vector<int64_t>>& vec)
{
    int64_t max = numeric_limits<int64_t>::min();
    for (const vector<int64_t>& nested_vec : vec)
    {
        for (const int64_t& val : nested_vec)
        {
            if (max < val)
            {
                max = val;
            }
        }
    }
    return max;
}


void write_123_bijection(const int n, ostream& out)
{
    vector<vector<perm_t>> avoiders = pattern_avoiding_permutations("123", n);
    sort(avoiders[n].begin(), avoiders[n].end(), [n](perm_t lhs, perm_t rhs) { return compare_perm(lhs, rhs, n); });
    const vector<perm_t>::iterator bound = stable_partition(avoiders[n].begin(), avoiders[n].end(), [n](const perm_t perm) { return is_even(perm, n); });

    out << "E_" << n << "(123) without bijection to even R_" << n + 1 << endl << endl;
    for (vector<perm_t>::iterator it = avoiders[n].begin(); it != bound; ++it)
    {
        const vector<int> ascents = get_ascents(*it, n);
        const int first_ascent = ascents.empty() ? n - 1 : ascents[0];
        if (first_ascent % 2 == 1)
        {
            out << perm_to_str(*it, n) << "  " << first_ascent + 1 << endl;
        }
    }
    out <<"\n\nEven R_" << n + 1 << "(123) without bijection to E_" << n << endl << endl;
    for (vector<perm_t>::iterator it = bound; it != avoiders[n].end(); ++it)
    {
        const vector<int> ascents = get_ascents(*it, n);
        const int first_ascent = ascents.empty() ? n - 1 : ascents[0];
        if (first_ascent % 2 == 1)
        {
            const perm_t bijected_perm = add_digit(*it, first_ascent + 1, n);
            out << perm_to_str(bijected_perm, n + 1) << "  " << first_ascent + 1 << endl;
        }
    }
    out <<"\n\nO_" << n << "(123) without bijection to odd R_" << n + 1 << endl << endl;
    for (vector<perm_t>::iterator it = bound; it != avoiders[n].end(); ++it)
    {
        const vector<int> ascents = get_ascents(*it, n);
        const int first_ascent = ascents.empty() ? n - 1 : ascents[0];
        if (first_ascent % 2 == 1)
        {
            out << perm_to_str(*it, n) << "  " << first_ascent + 1 << endl;
        }
    }
    out <<"\n\nOdd R_" << n + 1 << "(123) without bijection to O_" << n << endl << endl;
    for (vector<perm_t>::iterator it = avoiders[n].begin(); it != bound; ++it)
    {
        const vector<int> ascents = get_ascents(*it, n);
        const int first_ascent = ascents.empty() ? n - 1 : ascents[0];
        if (first_ascent % 2 == 1)
        {
            const perm_t bijected_perm = add_digit(*it, first_ascent + 1, n);
            out << perm_to_str(bijected_perm, n + 1) << "  " << first_ascent + 1 << endl;
        }
    }
}


void write_first_ascent_at_even_position(const int n, ostream& out)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted("123", n);
    vector<int64_t> even(n), odd(n);
    for (const perm_t& perm : avoiders[n])
    {
        const vector<int> ascents = get_ascents(perm, n);
        const int first_ascent = ascents.empty() ? n - 1 : ascents[0];
        if (is_even(perm, n))
        {
            even[first_ascent] += 1;
        }
        else
        {
            odd[first_ascent] += 1;
        }
    }
    const int max_even = floor(1 + log10(*max_element(even.begin(), even.end())));
    const int max_odd = floor(1 + log10(*max_element(odd.begin(), odd.end())));
    const int padding = max(max_even, max_odd);
    out << left;
    out << "n=" << setw(3) << n;
    for (int i = 2; i <= n; i += 2)
    {
        out << setw(padding + 1) << i;
    }
    out << endl;
    out << setw(5) << "even ";
    for (size_t i = 1; i < even.size(); i += 2)
    {
        out << setw(padding + 1) << even[i];
    }
    out << endl;
    out << setw(5) << "odd  ";
    for (size_t i = 1; i < odd.size(); i += 2)
    {
        out << setw(padding + 1) << odd[i];
    }
    out << endl;
}


void write_first_ascent_at_second_position(const int n, ostream& out)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted("123", n);
    vector<perm_t> even_abnX, odd_abnX;
    for (const perm_t& perm : avoiders[n])
    {
        if (get_first_ascent(perm, n) == 1)
        {
            if (is_even(perm, n))
            {
                even_abnX.push_back(perm);
            }
            else
            {
                odd_abnX.push_back(perm);
            }
        }
    }
    out << "Even " << even_abnX.size() << endl << endl;
    for (const perm_t& perm : even_abnX)
    {
        out << perm_to_str(perm, n) << endl;
    }
    out << endl << "Odd " << odd_abnX.size() << endl << endl;
    for (const perm_t& perm : odd_abnX)
    {
        out << perm_to_str(perm, n) << endl;
    }
    // E(abX) and O(abX)
    vector<perm_t> even_abX, odd_abX;
    for (const perm_t& perm : avoiders[n - 1])
    {
        if (get_first_ascent(perm, n - 1) != 0)
        {
            if (is_even(perm, n - 1))
            {
                even_abX.push_back(perm);
            }
            else
            {
                odd_abX.push_back(perm);
            }
        }
    }
    out << endl << endl << "Even " << even_abX.size() << endl << endl;
    for (const perm_t& perm : even_abX)
    {
        out << perm_to_str(perm, n - 1) << endl;
    }
    out << endl << "Odd " << odd_abX.size() << endl << endl;
    for (const perm_t& perm : odd_abX)
    {
        out << perm_to_str(perm, n - 1) << endl;
    }
}


void write_first_ascent(const int n, ostream& out)
{
    vector<vector<perm_t>> avoiders = pattern_avoiding_permutations("123", n);
    sort(avoiders[n].begin(), avoiders[n].end(), [n](perm_t lhs, perm_t rhs) { return compare_perm(lhs, rhs, n); });
    const vector<perm_t>::iterator bound = stable_partition(avoiders[n].begin(), avoiders[n].end(), [n](const perm_t perm) { return is_even(perm, n); });
    out << "Even" << endl << endl;
    for (vector<perm_t>::iterator it = avoiders[n].begin(); it != bound; ++it)
    {
        const int first_ascent = get_first_ascent(*it, n);
        out << perm_to_str(*it, n) << "  " << first_ascent << endl;
    }
    out << endl << endl << "Odd" << endl << endl;
    for (vector<perm_t>::iterator it = bound; it != avoiders[n].end(); ++it)
    {
        const int first_ascent = get_first_ascent(*it, n);
        out << perm_to_str(*it, n) << "  " << first_ascent << endl;
    }
}


void write_first_ascent(const int n_min, const int n_max, ostream& out)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted("123", n_max);
    vector<vector<int64_t>> even(avoiders.size());
    vector<vector<int64_t>> odd(avoiders.size());
    for (int i = n_min; i <= n_max; i++)
    {
        even[i] = vector<int64_t>(i);
        odd[i] = vector<int64_t>(i);
        for (const perm_t& perm : avoiders[i])
        {
            const int first_ascent = get_first_ascent(perm, i);
            if (is_even(perm, i))
            {
                even[i][first_ascent] += 1;
            }
            else
            {
                odd[i][first_ascent] += 1;
            }
        }
    }
    const int padding_even = ceil(log10(max_vector2d(even)));
    const int padding_odd = ceil(log10(max_vector2d(odd)));
    const int padding_n = ceil(log10(n_max));
    out << left;
    out << setw(padding_n + 2) << "";
    for (int i = 1; i <= n_max; i++)
    {
        out << setw(padding_even + padding_odd + 4) << i;
    }
    out << endl;
    for (int i = n_min; i <= n_max; i++)
    {
        out << setw(padding_n + 2) << i;
        for (size_t j = 0; j < even[i].size(); j++)
        {
            out << setw(padding_even + 1) << even[i][j] << setw(padding_odd + 3) << odd[i][j];
        }
        out << endl;
    }
}


void write_first_ascent_diff(const int n_min, const int n_max, ostream& out)
{
    const vector<vector<perm_t>> avoiders = pattern_avoiding_permutations_sorted("123", n_max);
    vector<vector<int64_t>> even(avoiders.size());
    vector<vector<int64_t>> odd(avoiders.size());
    vector<vector<int64_t>> diff(avoiders.size());
    for (int i = n_min; i <= n_max; i++)
    {
        even[i] = vector<int64_t>(i);
        odd[i] = vector<int64_t>(i);
        diff[i] = vector<int64_t>(i);
        for (const perm_t& perm : avoiders[i])
        {
            const int first_ascent = get_first_ascent(perm, i);
            if (is_even(perm, i))
            {
                even[i][first_ascent] += 1;
            }
            else
            {
                odd[i][first_ascent] += 1;
            }
        }
        for (size_t j = 0; j < diff[i].size(); j++)
        {
            diff[i][j] = even[i][j] - odd[i][j];
        }
    }
    // const int padding_even = ceil(log10(max_vector2d(even)));
    // const int padding_odd = ceil(log10(max_vector2d(odd)));
    const int padding_diff = ceil(log10(max_vector2d(diff))) + 1;
    const int padding_n = ceil(log10(n_max));
    // out << left;
    // out << setw(padding_n + 2) << "";
    // for (int i = 1; i <= n_max; i++)
    // {
    //     out << setw(padding_even + padding_odd + 4) << i;
    // }
    // out << endl;
    // for (int i = n_min; i <= n_max; i++)
    // {
    //     out << setw(padding_n + 2) << i;
    //     for (size_t j = 0; j < even[i].size(); j++)
    //     {
    //         out << setw(padding_even + 1) << even[i][j] << setw(padding_odd + 3) << odd[i][j];
    //     }
    //     out << endl;
    // }
    // out << endl << endl;
    out << setw(padding_n) << "";
    for (int i = 1; i <= n_max; i++)
    {
        out << setw(padding_diff + 1) << i;
    }
    out << endl << endl;
    for (int i = n_min; i <= n_max; i++)
    {
        out << setw(padding_n) << i;
        for (size_t j = 0; j < diff[i].size(); j++)
        {
            out << setw(padding_diff + 1) << diff[i][j];
        }
        out << endl;
    }
}


int main()
{
    string path = "/home/manuel/Documents/Semester 8/Bachelor Thesis/code/123 bijection/";
    // for (int i = 3; i <= 5; i += 2)
    // {
    //     ofstream file(path + "S_" + to_string(i) + "(123) bijection.txt");
    //     write_123_bijection(i, file);
    // }
    // for (int i = 3; i <= 5; i++)
    // {
    //     ofstream file(path + "S_" + to_string(i) + "(123) first ascent.txt");
    //     write_first_ascent(i, file);
    // }
    write_first_ascent_diff(3, 12, cout);
    // for (int i = 3; i <= 12; i++)
    // {
    //     if (i > 3)
    //     {
    //         cout << endl;
    //     }
    //     write_first_ascent_at_even_position(i, cout);
    // }
    // write_first_ascent_at_second_position(7, cout);
    return 0;
}
