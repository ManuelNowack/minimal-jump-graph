#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

int64_t even_permutation_size(const int n, const int k);
int64_t odd_permutation_size(const int n, const int k);
int64_t even_permutation_size_paper(const int n, const int k);
int64_t odd_permutation_size_paper(const int n, const int k);


/** Compute |E_n,k| that is the number of even permutations of length n with
    the first ascent at position k (one-based indexing).
*/
int64_t even_permutation_size(const int n, const int k)
{
    if (k > n || n <= 0 || k <= 0)
    {
        return 0;
    }
    // base case
    if (n == 1)
    {
        return 1;
    }
    int64_t result = 0;
    // insert n at position k + 1 which changes inversion number by n - k - 1
    if ((n - k - 1) % 2 == 0)
    {
        for (int i = k; i <= n; i++)
        {
            result += even_permutation_size(n - 1, i);
        }
    }
    else
    {
        for (int i = k; i <= n; i++)
        {
            result += odd_permutation_size(n - 1, i);
        }
    }
    // insert n at position 1 which changes inversion number by n - 1
    if ((n - 1) % 2 == 0)
    {
        result += even_permutation_size(n - 1, k - 1);
    }
    else
    {
        result += odd_permutation_size(n - 1, k - 1);
    }
    return result;
}


/** Compute |E_n,k| that is the number of odd permutations of length n with
    the first ascent at position k (one-based indexing).
*/
int64_t odd_permutation_size(const int n, const int k)
{
    if (k > n || n <= 0 || k <= 0)
    {
        return 0;
    }
    // base case
    if (n == 1)
    {
        return 0;
    }
    int64_t result = 0;
    // insert n at position k + 1 which changes inversion number by n - k - 1
    if ((n - k - 1) % 2 == 0)
    {
        for (int i = k; i <= n; i++)
        {
            result += odd_permutation_size(n - 1, i);
        }
    }
    else
    {
        for (int i = k; i <= n; i++)
        {
            result += even_permutation_size(n - 1, i);
        }
    }
    // insert n at position 1 which changes inversion number by n - 1
    if ((n - 1) % 2 == 0)
    {
        result += odd_permutation_size(n - 1, k - 1);
    }
    else
    {
        result += even_permutation_size(n - 1, k - 1);
    }
    return result;
}


void dynamic_programming_1234(vector<vector<vector<int64_t>>>& even, vector<vector<vector<int64_t>>>& odd, const int n)
{
    even = vector<vector<vector<int64_t>>>();
    odd = vector<vector<vector<int64_t>>>();
    for (size_t i = 0; i < (size_t) n + 1; i++)
    {
        even.push_back(vector<vector<int64_t>>(i + 1, vector<int64_t>(i + 1)));
        odd.push_back(vector<vector<int64_t>>(i + 1, vector<int64_t>(i + 1)));
    }
    even[1][1][1] = 1;
    for (size_t n = 1; n < even.size() - 1; n++)
    {
        for (size_t k1 = 1; k1 < even[n].size(); k1++)
        {
            for (size_t k2 = k1; k2 < even[n][k1].size(); k2++)
            {
                for (size_t i = 1; i <= k2 + 1; i++)
                {
                    const bool change_sign = (n - 1 - i) % 2 == 1;
                    if (i <= k1)
                    {
                        if (change_sign)
                        {
                            odd[n + 1][k1 + 1][k2 + 1] += even[n][k1][k2];
                            even[n + 1][k1 + 1][k2 + 1] += odd[n][k1][k2];
                        }
                        else
                        {
                            even[n + 1][k1 + 1][k2 + 1] += even[n][k1][k2];
                            odd[n + 1][k1 + 1][k2 + 1] += odd[n][k1][k2];
                        }
                    }
                    // else if (i <= k1)
                    // {
                    //     if (change_sign)
                    //     {
                    //         odd[n + 1][i - 1][k1 + 1] += even[n][k1][k2];
                    //         even[n + 1][i - 1][k1 + 1] += odd[n][k1][k2];
                    //     }
                    //     else
                    //     {
                    //         even[n + 1][i - 1][k1 + 1] += even[n][k1][k2];
                    //         odd[n + 1][i - 1][k1 + 1] += odd[n][k1][k2];
                    //     }
                    // }
                    else if (i == k1 + 1)
                    {
                        if (change_sign)
                        {
                            odd[n + 1][i - 1][k2 + 1] += even[n][k1][k2];
                            even[n + 1][i - 1][k2 + 1] += odd[n][k1][k2];
                        }
                        else
                        {
                            even[n + 1][i - 1][k2 + 1] += even[n][k1][k2];
                            odd[n + 1][i - 1][k2 + 1] += odd[n][k1][k2];
                        }
                    }
                    else
                    {
                        if (change_sign)
                        {
                            odd[n + 1][k1][i - 1] += even[n][k1][k2];
                            even[n + 1][k1][i - 1] += odd[n][k1][k2];
                        }
                        else
                        {
                            even[n + 1][k1][i - 1] += even[n][k1][k2];
                            odd[n + 1][k1][i - 1] += odd[n][k1][k2];
                        }
                    }
                }
            }
        }
    }
}


void dynamic_programming_123(vector<vector<int64_t>>& even, vector<vector<int64_t>>& odd, const int n)
{
    even = vector<vector<int64_t>>();
    odd = vector<vector<int64_t>>();
    for (size_t i = 0; i < (size_t) n + 1; i++)
    {
        even.push_back(vector<int64_t>(i + 1));
        odd.push_back(vector<int64_t>(i + 1));
    }
    even[1][1] = 1;
    for (size_t n = 1; n < even.size() - 1; n++)
    {
        for (size_t k = 1; k < even[n].size(); k++)
        {
            for (size_t i = 1; i <= k + 1; i++)
            {
                const bool change_sign = (n - 1 - i) % 2 == 1;
                if (i == 1)
                {
                    if (change_sign)
                    {
                        odd[n + 1][k + 1] += even[n][k];
                        even[n + 1][k + 1] += odd[n][k];
                    }
                    else
                    {
                        even[n + 1][k + 1] += even[n][k];
                        odd[n + 1][k + 1] += odd[n][k];
                    }
                }
                else
                {
                    if (change_sign)
                    {
                        odd[n + 1][i - 1] += even[n][k];
                        even[n + 1][i - 1] += odd[n][k];
                    }
                    else
                    {
                        even[n + 1][i - 1] += even[n][k];
                        odd[n + 1][i - 1] += odd[n][k];
                    }
                }
            }
        }
    }
}


int main()
{
    vector<vector<vector<int64_t>>> even, odd;
    dynamic_programming_1234(even, odd, 5);
    for (size_t n = 1; n < even.size(); n++)
    {
        int64_t sum_even = 0;
        int64_t sum_odd = 0;
        // for (size_t k = 0; k < even[n].size(); k++)
        // {
        //     if (even[n][k] != even_permutation_size(n, k))
        //     {
        //         cout << "E_" << n << "," << k << " = " << even[n][k];
        //         cout << "  real value is " << even_permutation_size(n, k) << endl;
        //     }
        //     if (odd[n][k] != odd_permutation_size(n, k))
        //     {
        //         cout << "O_" << n << "," << k << " = " << odd[n][k];
        //         cout << "  real value is " << odd_permutation_size(n, k) << endl;
        //     }
        //     sum_even += even[n][k];
        //     sum_odd += odd[n][k];
        // }
        for (size_t k1 = 0; k1 < even[n].size(); k1++)
        {
            for (size_t k2 = 0; k2 < even[n][k1].size(); k2++)
            {
                // cout << "E_" << n << "," << k1 << "," << k2 << " = " << even[n][k1][k2] << endl;
                // cout << "O_" << n << "," << k1 << "," << k2 << " = " << odd[n][k1][k2] << endl;
                sum_even += even[n][k1][k2];
                sum_odd += odd[n][k1][k2];
            }
            // cout << endl;
        }
        // cout << "E_" << n << " = "  << sum_even << endl;
        // cout << "O_" << n << " = "  << sum_odd << endl;
        cout << "S_" << n << " = "  << sum_even + sum_odd << endl;
    }
    return 0;
}
