#ifndef _PATTERN_AVOIDANCE_H 
#define _PATTERN_AVOIDANCE_H

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include "perm.h"
using namespace std;  // perm.h already enforces namespace std...


namespace pattern_avoidance {

/** Inserts a new digit into a permutation at a specific position.
*/
inline perm_t add_digit(const perm_t perm, const int pos, const uint64_t new_digit)
{
    return setdigit(addpos(perm, pos), pos, new_digit);
}


/** Appends all non-duplicate permutations of length n to each string of a
    vector.

    For example calling append_all_permutations({"123, 321"}, 3) returns
    {"123", "123 132", "123 213", "123 231", "123 312", "123 321",
     "321", "321 123", "321 132", "321 213", "321 231", "321 312"}.

    Asserts n <= 20 because n! cannot be stored in a 64-bit integer for larger
    values.
*/
vector<string> append_all_permutations(const vector<string>& patterns, const int n);


/** Returns true if and only if the permutation lhs is lexographically smaller
    than permutations rhs. Both permutations must be of length n.

    TODO: Do we even need this? The < comparison should work if all unused data
    in the permutation are zero bits.
*/
bool compare_perm(const perm_t lhs, const perm_t rhs, const int n);


/** Returns all permutations on the set {1, ..., n}.

    Asserts n <= 20 because n! cannot be stored in a 64-bit integer for larger
    values.
*/
vector<perm_t> compute_all_permutations(const int n);


/** Returns all permutations on the set {1, ..., n} as simple strings, i.e.
    there is no separator between digits.

    Asserts n < 10 because permutations of length n >= 10 have ambiguous
    representation as simple strings.
*/
vector<string> compute_all_permutations_simple_string(const int n);


/** Returns all ascents in a permutation of length n.
    An ascent is the position of a digit that is followed by a larger digit.
    Position index starts at zero.

    For example the ascents of 1324 are 0 and 2.
*/
vector<int> get_ascents(const perm_t perm, const int n);


/** Returns the number of ascents in a permutation of length n.
*/
int get_num_ascents(const perm_t perm, const int n);


/** Returns the position of a digit in a permutation. Position index starts at
    zero. Returns -1 if the permutation does not contain the digit.
*/
int get_digit_position(const perm_t perm, const uint64_t digit);


/** Returns the inversion number of a permutation of length n.
*/
int inversion_number(const perm_t perm, const int n);


/** Returns the identity permutation (increasing sequence) of length n.
*/
perm_t identity_permutation(const int n);


/** Returns true if and only if a permutation has an even inversion number.
*/
inline bool is_even(const perm_t perm, const int n)
{
    return inversion_number(perm, n) % 2 == 0;
}


/** Returns true if and only if a permutation is tame, i.e. the largest digit
    is not at the left or right-most position.
*/
inline bool is_tame(const perm_t perm, const int n)
{
    const uint64_t max_digit = n - 1;
    return getdigit(perm, 0) != max_digit && getdigit(perm, n - 1) != max_digit;
}


/** Returns all permutations up to length n that avoid the pattern.
    
    The return value is a two-dimensional vector where vec[i] contains all patterns
    of length i in no particular order.
*/
vector<vector<perm_t>> pattern_avoiding_permutations(const string pattern, const int n);


/** Returns all permutations up to length n that avoid the pattern.
    
    The return value is a two-dimensional vector where vec[i] contains all patterns
    of length i in lexicographical order.
*/
vector<vector<perm_t>> pattern_avoiding_permutations_sorted(const string pattern, const int n);


/** Returns a string representation of a permutation of length n. Each digit is
    separated by a whitespace.

    For example 1324 is represented as "1 3 2 4".
*/
string perm_to_str(const perm_t perm, const int n);


/** Returns a string representation of a permutation of length n. There is no
    separator between digits. Therefore the representation is ambiguous for
    permutations of length 10 and larger.
*/
string perm_to_str_simple(const perm_t perm, const int n);


/** Returns the permutation corresponding to a simple string.
*/
perm_t string_to_permutation(const string permutation);

} // namespace pattern_avoidance

#endif
