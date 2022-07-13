#include <cstdint>
#include <iostream>
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


int64_t even_permutation_size_paper(const int n, const int k)
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
    if (n % 2 == 0 && k % 2 == 0)
    {
        result += odd_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += odd_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 0 && k % 2 == 1)
    {
        result += odd_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += even_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 1 && k % 2 == 0)
    {
        result += even_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += even_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 1 && k % 2 == 1)
    {
        result += even_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += odd_permutation_size_paper(n - 1, i);
        }
    }
    return result;
}


int64_t odd_permutation_size_paper(const int n, const int k)
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
    if (n % 2 == 0 && k % 2 == 0)
    {
        result += even_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += even_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 0 && k % 2 == 1)
    {
        result += even_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += odd_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 1 && k % 2 == 0)
    {
        result += odd_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += odd_permutation_size_paper(n - 1, i);
        }
    }
    else if (n % 2 == 1 && k % 2 == 1)
    {
        result += odd_permutation_size_paper(n - 1, k - 1);
        for (int i = k; i < n; i++)
        {
            result += even_permutation_size_paper(n - 1, i);
        }
    }
    return result;
}


int64_t even_permutation_size_test(const int n)
{
    int64_t result = 0;
    // for (int k = 2; k <= n; k += 2)
    // {
    //     result += odd_permutation_size(n - 1, k - 1);
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += odd_permutation_size(n - 1, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     result += odd_permutation_size(n - 1, k - 1);
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // for (int k = 1; k <= n; k++)
    // {
    //     result += odd_permutation_size(n - 1, k - 1);
    // }
    // for (int k = 2; k <= n; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += odd_permutation_size(n - 1, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // first term replaced
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += odd_permutation_size(n - 2, k);
    // }
    // for (int k = 2; k <= n - 2; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 2; k <= n; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += odd_permutation_size(n - 1, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // second term replaced
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += odd_permutation_size(n - 2, k);
    // }
    // for (int k = 2; k <= n - 2; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 2; k <= n; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k + 1; i <= n - 1; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // third term replaced
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += odd_permutation_size(n - 2, k);
    // }
    // for (int k = 2; k <= n - 2; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 2; k <= n; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k + 1; i <= n - 1; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 1; i++)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // third term, second success
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    //     for (int i = k; i <= n - 1; i += 2)
    //     {
    //         result += even_permutation_size(n - 1, i);
    //     }
    // }

    // third term, third success
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         result += even_permutation_size(n - 2, i - 1);
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 1; i += 2)
    //     {
    //         result += even_permutation_size(n - 2, i - 1);
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    // third term, fourth success
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k; i <= n - 1; ++i)
    //     {
    //         result += even_permutation_size(n - 2, i - 1);
    //     }
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 1; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    // third term, penultimate success
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k - 1; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 1; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    // third term, final success
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += even_permutation_size(n - 2, k);
    // }
    // for (int k = 2; k <= n - 2; k += 2)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 1; k += 2)
    // {
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 1; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }


    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += even_permutation_size(n - 2, k);
    // }
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += odd_permutation_size(n - 2, k);
    // }

    // for (int k = 1; k <= n - 2; k++)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //     }
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }

    // for (int k = 1; k <= n - 2; k++)
    // {
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    // 2,3,4
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     result += even_permutation_size(n - 2, k);
    //     result += odd_permutation_size(n - 2, k);
    // }
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     for (int i = k; i <= n - 2; i++)
    //     {
    //         result += odd_permutation_size(n - 2, i);
    //         result += even_permutation_size(n - 2, i);
    //     }
    // }
    // for (int k = 1; k <= n - 2; k++)
    // {
    //     for (int i = k + 1; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += even_permutation_size(n - 2, j);
    //         }
    //     }
    //     for (int i = k; i <= n - 2; i += 2)
    //     {
    //         for (int j = i; j <= n - 2; j++)
    //         {
    //             result += odd_permutation_size(n - 2, j);
    //         }
    //     }
    // }

    for (int k = 1; k <= n - 2; k++)
    {
        result += even_permutation_size(n - 2, k);
        result += odd_permutation_size(n - 2, k);
    }
    for (int k = 1; k <= n - 2; k++)
    {
        for (int i = k; i <= n - 2; i++)
        {
            result += odd_permutation_size(n - 2, i);
            result += even_permutation_size(n - 2, i);
        }
    }
    for (int i = 1; i <= n - 3; i += 2)
    {
        for (int j = i; j <= n - 2; j++)
        {
            result += odd_permutation_size(n - 2, j);
        }
    }
    for (int k = 2; k <= n - 2; k++)
    {
        for (int i = k; i <= n - 2; i += 2)
        {
            for (int j = i; j <= n - 2; j++)
            {
                result += even_permutation_size(n - 2, j);
            }
        }
        for (int i = k; i <= n - 2; i += 2)
        {
            for (int j = i; j <= n - 2; j++)
            {
                result += odd_permutation_size(n - 2, j);
            }
        }
    }

    return result;
}


int main()
{
    string path = "/home/manuel/Documents/Semester 8/Bachelor Thesis/code/123 bijection/";
    const int n_min = 2;
    const int n_max = 12;
    for (int n = n_min; n <= n_max; n += 2)
    {
        if (n > n_min)
        {
            cout << endl;
        }
        cout << n << ": " << even_permutation_size_test(n) << endl;
        // int64_t E_n = 0;
        // int64_t O_n = 0;
        // for (int k = 1; k <= n; k++)
        // {
            // E_n += even_permutation_size(n, k);
            // O_n += odd_permutation_size(n, k);
            // if (even_permutation_size(n, k) != even_permutation_size_paper(n, k))
            // {
            //     cout << "failed for E_" << n << "," << k << endl;
            // }
            // if (odd_permutation_size(n, k) != odd_permutation_size_paper(n, k))
            // {
            //     cout << "failed for O_" << n << "," << k << endl;
            // }
            // cout << "E_" << n << "," << k << " = " << even_permutation_size(n, k) << endl;
        // }
        // cout << "E_" << n << " = " << E_n << endl;
        // cout << "O_" << n << " = " << O_n << endl;
    }
    return 0;
}
