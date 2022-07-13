#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


typedef struct parity_count_t {
    int64_t even;
    int64_t odd;
    parity_count_t(int64_t even, int64_t odd)
        : even(even)
        , odd(odd)
    { }
} parity_count_t;


/** Returns true if and only if a permutation of digits avoids the increasing
    pattern of length k.
*/
bool avoids_increasing_pattern(const vector<int>& digits, const int k)
{
    // contains the largest increasing subsequence up to this digit
    vector<int> largest_subsequence(digits.size(), 1);
    for (size_t i = 0; i < digits.size(); i++)
    {
        for (size_t j = i + 1; j < digits.size(); j++)
        {
            if (digits[i] < digits[j])
            {
                if (largest_subsequence[j] < largest_subsequence[i] + 1)
                {
                    largest_subsequence[j] = largest_subsequence[i] + 1;
                    if (largest_subsequence[j] >= k)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}


/** Returns the inversion number of a permutation of digits.
*/
int64_t inversion_number(const vector<int>& digits)
{
    int64_t result = 0;
    for (size_t i = 0; i < digits.size(); i++)
    {
        for (size_t j = i + 1; j < digits.size(); j++)
        {
            if (digits[i] > digits[j])
            {
                result++;
            }
        }
    }
    return result;
}


/** Returns the number of even and odd permutations of length n that avoid the
    increasing pattern of length k.
*/
parity_count_t count_parity(const int n, const int k)
{
    vector<int> digits(n);
    for (int i = 0; i < n; i++)
    {
        digits[i] = i;
    }
    int64_t even_count = 0;
    int64_t odd_count = 0;
    do {
        if (avoids_increasing_pattern(digits, k))
        {
            if (inversion_number(digits) % 2 == 0)
            {
                even_count += 1;
            }
            else
            {
                odd_count += 1;
            }
        }
    }
    while (next_permutation(digits.begin(), digits.end()));
    return parity_count_t(even_count, odd_count);
}


void write_even_odd_difference(const int n_max, const int k, ostream& out)
{
    vector<int64_t> even(n_max + 1), odd(n_max + 1), diff(n_max + 1);
    for (int i = k; i <= n_max; i++)
    {
        const parity_count_t parity_count = count_parity(i, k);
        even[i] = parity_count.even;
        odd[i] = parity_count.odd;
        diff[i] = abs(parity_count.even - parity_count.odd);
    }
    const string title_n = "n";
    const string title_even = "even";
    const string title_odd = "odd";
    const string title_diff = "difference";
    const string title_larger = "larger";
    const int max_n_len = to_string(n_max).length();
    const int max_even_len = to_string(*max_element(even.begin(), even.end())).length();
    const int max_odd_len = to_string(*max_element(odd.begin(), odd.end())).length();
    const int max_diff_len = to_string(*max_element(diff.begin(), diff.end())).length();
    const int padding_n = max(max_n_len, (int) title_n.length());
    const int padding_even = max(max_even_len, (int) title_even.length());
    const int padding_odd = max(max_odd_len, (int) title_odd.length());
    const int padding_diff = max(max_diff_len, (int) title_diff.length());
    const int padding_larger = title_larger.length();
    const int column_dist = 2;
    for (int i = 1; i <= k; i++)
    {
        out << i;
    }
    out << endl;
    out << left;
    out << setw(padding_n + column_dist) << title_n;
    out << setw(padding_even + column_dist) << title_even;
    out << setw(padding_odd + column_dist) << title_odd;
    out << setw(padding_diff + column_dist) << title_diff;
    out << setw(padding_larger + column_dist) << title_larger;
    out << endl;
    for (int i = k; i <= n_max; i++)
    {
        out << setw(padding_n + column_dist) << i;
        out << setw(padding_even + column_dist) << even[i];
        out << setw(padding_odd + column_dist) << odd[i];
        out << setw(padding_diff + column_dist) << diff[i];
        if (even[i] - odd[i] > 0)
        {
            out << setw(padding_larger + column_dist) << "even";
        }
        else if (even[i] - odd[i] < 0)
        {
            out << setw(padding_larger + column_dist) << "odd";
        }
        else
        {
            out << setw(padding_larger + column_dist) << "-";
        }
        out << endl;
    }
}


int main()
{
    // this will run for quite some time
    constexpr int max_permutation_length = 14;
    constexpr int min_pattern_length = 3;
    constexpr int max_pattern_length = 6;
    for (int i = min_pattern_length; i <= max_pattern_length; i++)
    {
        write_even_odd_difference(max_permutation_length, i, cout);
        cout << endl;
    }
    return 0;
}
