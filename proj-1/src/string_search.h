/**
 * Implementation of multiple string search algorithms.
 *
 * P : The pattern to match in a given string, or its length, or the preprocessed pattern.
 * T : The string to be searched by a given pattern, or its length.
 * L : The alphabet of T (we use ASCII 8-bit), or its cardinality
 * E : The set of characters of P (subset of L), or its cardinality
 * A : The string to be transformed in an edit distance algorithm, or its length.
 * B : The target string in an edit distance algorithm, or its length.
 *
 * ***** Functions included:
 *
 * *** Exact string search:
 *   naive_search(text, pattern)
 *   boyer_moore_search(text, boyer_moore(pattern))
 *   boyer_moore_galil_search(text, boyer_moore(pattern))
 *   knuth_morris_pratt_search(text, knuth_morris_pratt(pattern))
 *
 * *** Approximate match (edit distance):
 *   hamming_distance(a, b)
 *   levenshtein_distance(a, b)
 *   restricted_damerau_distance(a, b)
 *   damerau_distance(a, b)
 *
 * *** Fuzzy search:
 *   levenshtein_fuzzy(text, pattern)
 *   restricted_damerau_fuzzy(text, pattern)
 *   damerau_fuzzy(text, pattern)
 */
#ifndef STRING_SEARCH_H___
#define STRING_SEARCH_H___

#include "substring.h"

#include <string>
#include <vector>

namespace search {

template <typename T>
using exact_search_t = std::vector<std::size_t>(*)(const std::string&, const T&);
// return: vector of indices;   1st arg: text (T);   2nd arg: pattern (maybe preprocessed) (P).

using distance_function_t = std::size_t(*)(const std::string&, const std::string&);
// return: distance;   1st arg: A;   2nd arg: B.

using fuzzy_search_t = std::size_t(*)(const std::string&, const std::string&);
// return: distance;   1st arg: text (T, B);   2nd arg: pattern (P, A).



/**
 * *******************
 * EXACT STRING SEARCH
 * *******************
 */

/**
 * Naive search
 *
 * Preprocessing time:
 *     None
 * Search time complexity:
 *     O(T * P)  worst case
 *     O(T - P)  best case
 *     O(T)      average case for a random T.
 * Space complexity:
 *     O(1)
 */
std::vector<std::size_t> naive_search(const std::string& text, const std::string& pattern);

/**
 * Boyer-Moore search
 *
 * We start by implementing helper classes for the bad character
 * rule and the good suffix rule.
 *
 * We provide three different classes for the bad character rule:
 *     bad_char_table_rule -- lookup table
 *         Preprocessing: O(P ** 2)
 *         Space:         O(L * P)
 *     bad_char_map_rule   -- lookup-lookup table
 *         Preprocessing: O(P ** 2)
 *         Space:         O(E * P)
 *     bad_char_list_rule  -- last occurrence list
 *         Preprocessing: O(P)
 *         Space:         O(P)
 *
 * The second is an improvement on the first. The third is the classic
 * bad character list with fast preprocessing time, but the fact that
 * only the last occurrence of each character is stored means that it
 * returns 1-shifts when the mismatch character last appears later
 * in the pattern.
 *
 * The boyer_moore template class wraps the bad character rule and
 * the good character rule. It takes the desired bad character
 * implementation, and returns an object which implements the
 * shift function, taking the current index of the mismatch and
 * the bad character and returning the right shift the pattern should
 * make. It uses bad_char_map_rule by default.
 *
 * Preprocessing time:
 *     O(P ** 2)  classic
 *     O(P)       list
 * Search time complexity (without Galil rule):
 *     O(T * P)  worst case
 *     O(T / P)  best case
 * Search time complexity (with Galil rule):
 *     O(T)      worst case
 *     O(T / P)  best case
 * Space:
 *     O(L * P)  lookup table
 *     O(E * P)  lookup-lookup table
 *     O(P)      list
 */

/**
 * Base class of the bad_char_rule.
 */
class bad_char_rule {
public:
    virtual std::size_t shift(std::size_t index, char text_char) const = 0;
};

/**
 * Implements the bad character rule with a 2D table for the entire alphabet L.
 * 
 * Preprocessing time:
 *     O(P ** 2)
 * Lookup:
 *     O(1)
 * Space:
 *     O(L * P)
 */
class bad_char_table_rule : public bad_char_rule {
private:
    std::vector<std::vector<std::size_t>> matrix;
public:
    bad_char_table_rule(const std::string& pattern);
    std::size_t shift(std::size_t index, char text_char) const;
};

/**
 * If we add a level of indirection to matrix[text_char], making sure
 * our matrix table only stores actually used characters of L,
 * we keep our O(1) lookup but reduce space.
 *
 * Preprocessing time:
 *     O(P ** 2)
 * Lookup:
 *     O(1)
 * Space:
 *     O(E * P)
 */
class bad_char_map_rule : public bad_char_rule {
private:
    std::vector<std::size_t> mapping;
    std::vector<std::vector<std::size_t>> matrix;
public:
    bad_char_map_rule(const std::string& pattern);
    std::size_t shift(std::size_t index, char text_char) const;
};

/**
 * Bad character rule last occurrence list, for Horspool
 *
 * Preprocessing time:
 *     O(P + L) theoretical, might be O(P) implementation
 * Lookup:
 *     O(1)
 * Space:
 *     O(E)
 */
class bad_char_list_rule : public bad_char_rule {
private:
    std::vector<std::size_t> list;
public:
    bad_char_list_rule(const std::string& pattern);
    std::size_t shift(std::size_t index, char text_char) const;
};

/**
 * Implements the good suffix rule.
 *
 * Preprocessing time:
 *     O(P)
 * Lookup:
 *     O(1)
 * Space:
 *     O(P)
 */
class good_suffix_rule {
private:
    std::vector<std::size_t> lookup;
public:
    good_suffix_rule(const std::string& pattern);
    std::size_t shift(std::size_t index) const;
};

/**
 * Boyer-Moore preprocessor
 *
 * Preprocessing time:
 *     O(P) or O(P ** 2)
 * Lookup:
 *     O(1)
 * Space:
 *     O(P) or O(E * P) or O(L * P)
 */
template <typename bad_t = bad_char_map_rule, typename good_t = good_suffix_rule>
class boyer_moore {
    bad_t bad;
    good_t good;
    const std::string pattern;
public:
    boyer_moore(std::string pattern);
    std::size_t bad_shift(std::size_t index, char text_char) const;
    std::size_t good_shift(std::size_t index) const;
    std::size_t mismatch_shift(std::size_t index, char text_char) const;
    std::size_t match_shift() const;
    const std::string& get_pattern() const;
};

/**
 * Boyer-Moore search without Galil rule
 *
 * Search time complexity:
 *     O(T * P)  worst case
 *     O(T / P)  best case
 */
template <typename bad_t, typename good_t>
std::vector<std::size_t> boyer_moore_search(const std::string& text, const boyer_moore<bad_t, good_t>& bm);

/**
 * Boyer-Moore search with Galil rule
 * 
 * Search time complexity:
 *     O(T) worst case
 *     O(T / P) best case
 */
template <typename bad_t, typename good_t>
std::vector<std::size_t> boyer_moore_galil_search(const std::string& text, const boyer_moore<bad_t, good_t>& bm);

/**
 * Knuth-Morris-Pratt search
 */

/**
 * Knuth-Morris-Pratt preprocessor
 *
 * Preprocessing time:
 *     O(P)
 * Lookup:
 *     O(1)
 * Space:
 *     O(P)
 */
class knuth_morris_pratt {
    std::vector<int> lookup_;
    std::string pattern;
public:
    knuth_morris_pratt(std::string pattern);
    int lookup(std::size_t index) const;
    std::size_t shift(std::size_t index) const;
    const std::string& get_pattern() const;
};

/**
 * Knuth-Morris-Pratt search
 *
 * Search time complexity:
 *     O(T)
 */
std::vector<std::size_t> knuth_morris_pratt_search(const std::string& text, const knuth_morris_pratt& kmp);
/**
 * APPROXIMATE STRING MATCHING
 */

/**
 * Hamming distance of two strings. Equal size is enforced.
 *
 * Time complexity:
 *     O(min(S1, S2))
 * Space:
 *     O(1)
 */
std::size_t hamming_distance(const std::string& str1, const std::string& str2);





/**
 * APPROXIMATE STRING MATCHING -- EDIT DISTANCE
 * 
 * Edit distance between two strings a and b.
 *
 * The edit distance is directional: We compute edit_distance(a, b) considering the
 * insertions, removals, substitutions and transpositions happening __ in a __ while
 * b is static. So edit_distance(a, b) is not necessarily equal to edit_distance(b, a).
 * 
 * For example edit_distance("a", "") further down is remov_cost,
 * while edit_distance("", "a") is inser_cost.
 */

/**
 * Levenshtein distance computed using a AxB matrix. 
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(A * B)
 */
std::size_t matrix_levenshtein_distance(const std::string& a, const std::string& b);

/**
 * Levenshtein distance computed using a 2xmin(A,B) matrix.
 * This is possible because levenshtein(A, B) = levenshtein(B, A).
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(min(A, B))
 */
std::size_t vector_levenshtein_distance(const std::string& a, const std::string& b);

std::size_t levenshtein_distance(const std::string& a, const std::string& b);

/**
 * Restricted Damerau-Levenshtein distance computed using a AxB matrix
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(A * B)
 */
std::size_t matrix_restricted_damerau_distance(const std::string& a, const std::string& b);

/**
 * Restricted Damerau-Levenshtein distance computed using a 3xB matrix.
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(B)
 */
std::size_t vector_restricted_damerau_distance(const std::string& a, const std::string& b);

std::size_t restricted_damerau_distance(const std::string& a, const std::string& b);

/**
 * Damerau-Levenshtein distance.
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(A * B)
 */
std::size_t damerau_distance(const std::string& a, const std::string& b);





/**
 * FUZZY STRING SEARCH
 *
 * Variations of the approximate string matching algorithms, where
 * we find the substring of text (aka b) which has minimal edit distance
 * to pattern (aka a).
 */

/**
 * Levenshtein fuzzy search done using a PxT matrix.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T * P)
 */
std::size_t matrix_levenshtein_fuzzy(const std::string& text, const std::string& pattern);

/**
 * Levenshtein fuzzy search done using a 2xT matrix.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T)
 */
std::size_t vector_levenshtein_fuzzy(const std::string& text, const std::string& pattern);

std::size_t levenshtein_fuzzy(const std::string& text, const std::string& pattern);

/**
 * Restricted Damerau-Levenshtein fuzzy search done using a PxT matrix.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T * P)
 */

std::size_t matrix_restricted_damerau_fuzzy(const std::string& text, const std::string& pattern);

/**
 * Restricted Damerau-Levenshtein fuzzy search done using a 3xT matrix.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T)
 */
std::size_t vector_restricted_damerau_fuzzy(const std::string& text, const std::string& pattern);

std::size_t restricted_damerau_fuzzy(const std::string& text, const std::string& pattern);

/**
 * Damerau-Levenshtein fuzzy search.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T * P)
 */
std::size_t damerau_fuzzy(const std::string& text, const std::string& pattern);

}

// Template implementation
#include "string_search.ipp"

#endif // STRING_SEARCH_H___
