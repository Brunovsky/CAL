#ifndef MIN_PRIORITY_QUEUE_H___
#define MIN_PRIORITY_QUEUE_H___

#if 0

/**
 * Generic edit distance between two strings a and b internally using a AxB matrix.
 *     Insertions        cost: inser_cost, independent of the character
 *     Removals          cost: remov_cost, independent of the character
 *     Substitutions     cost: subst_cost, independent of the character
 *     Transpositions    cost: trans_cost, independent of the character
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(A * B)
 */
template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
class matrix_edit_distance : public edit_distance {
public:
    size_t distance(const string& a, const string& b);
}

template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
size_t matrix_edit_distance<inser_cost, remov_cost, subst_cost, trans_cost>
    ::distance(const string& a, const string& b) {
    size_t A = a.size(), B = b.size();

    vector<vector<size_t>> matrix(A + 1, vector<size_t>(B + 1));

    // matrix[i][j] is the smallest edit distance between
    // substring(a, 0, i) and substring(b, 0, j) (the first has length i, the second length j).
    // so we seek matrix[A][B].

    for (size_t i = 0; i <= A; ++i) {
        matrix[i][0] = remov_cost * i;
    }

    for (size_t j = 0; j <= B; ++j) {
        matrix[0][j] = inser_cost * j;
    }

    for (size_t i = 1; i <= A; ++i) {
        for (size_t j = 1; j <= B; ++j) {
            // total edit distance after an insertion of b[j-1] at a[i].
            size_t dist_inser = matrix[i][j - 1] + inser_cost;

            // total edit distance after the removal of a[i-1].
            size_t dist_remov = matrix[i - 1][j] + remov_cost;

            // total edit distance after a substitution of a[i-1] for b[j-1].
            // there is no cost if a[i-1] == b[j-1] obviously.
            size_t dist_subst = matrix[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : subst_cost);

            // smallest total edit distance so far.
            size_t best = std::min(dist_subst, std::min(dist_inser, dist_remov));

            // now consider transposition if a[i-2][i-1] is b[i-2][i-1] but transposed.
            // if it isn't do not consider transposition.
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                size_t dist_trans = matrix[i - 2][j - 2] + trans_cost;
                best = std::min(best, dist_trans);
            }

            // Store the best distance found.
            matrix[i][j] = best;
        }
    }

    return matrix[A][B];
}

/**
 * Specialization for the case where the transposition is not desired. Represented by a -1.
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(A * B)
 */
template <size_t inser_cost, size_t remov_cost, size_t subst_cost>
size_t matrix_edit_distance<inser_cost, remov_cost, subst_cost, -1>
    ::distance(const string& a, const string& b) {
    size_t A = a.size(), B = b.size();

    vector<vector<size_t>> matrix(A + 1, vector<size_t>(B + 1));

    for (size_t i = 0; i <= A; ++i) {
        matrix[i][0] = remov_cost * i;
    }

    for (size_t j = 0; j <= B; ++j) {
        matrix[0][j] = inser_cost * j;
    }

    for (size_t i = 1; i <= A; ++i) {
        for (size_t j = 1; j <= B; ++j) {
            size_t dist_inser = matrix[i][j - 1] + inser_cost;
            size_t dist_remov = matrix[i - 1][j] + remov_cost;
            size_t dist_subst = matrix[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : subst_cost);

            size_t best = std::min(dist_subst, std::min(dist_inser, dist_remov));

            // no transposition.

            matrix[i][j] = best;
        }
    }

    return matrix[A][B];
}

/**
 * Generic edit distance between two strings a and b internally using an 3xB matrix.
 *     Insertions        cost: inser_cost, independent of the character
 *     Removals          cost: remov_cost, independent of the character
 *     Substitutions     cost: subst_cost, independent of the character
 *     Transpositions    cost: trans_cost, independent of the character
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(B)
 */
template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
class vector_edit_distance : public edit_distance {
public:
    size_t distance(const string& a, const string& b);
}

template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
size_t vector_edit_distance<inser_cost, remov_cost, subst_cost, trans_cost>
    ::distance(const string& a, const string& b) {
    size_t A = a.size(), B = b.size();

    vector<vector<size_t>> matrix(3, vector<size_t>(B + 1));
    
    vector<size_t>& old = matrix[0];
    vector<size_t>& previous = matrix[1];
    vector<size_t>& current = matrix[2];

    for (size_t j = 0; j <= B; ++j) {
        previous[j] = inser_cost * j;
    }

    for (size_t i = 1; i <= A; ++i) {
        current[0] = remov_cost * i;

        for (size_t j = 1; j <= B; ++j) {
            // total edit distance after an insertion of b[j-1] at a[i].
            size_t dist_inser = current[j - 1] + inser_cost;

            // total edit distance after the removal of a[i-1].
            size_t dist_remov = previous[j] + remov_cost;

            // total edit distance after a substitution of a[i-1] for b[j-1].
            // there is no cost if a[i-1] == b[j-1] obviously.
            size_t dist_subst = previous[j - 1] + (a[i - 1] == b[j - 1] ? 0 : subst_cost);

            // smallest total edit distance so far.
            size_t best = std::min(dist_subst, std::min(dist_inser, dist_remov));

            // now consider transposition if a[i-2][i-1] is b[i-2][i-1] but transposed.
            // if it isn't do not consider transposition.
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                size_t dist_trans = old[j - 2] + trans_cost;
                best = std::min(best, dist_trans);
            }

            // Store the best distance found.
            current[j] = best;
        }

        old = matrix[i % 3];
        previous = matrix[(i + 1) % 3];
        current = matrix[(i + 2) % 3];
    }

    return previous[B];
}

/**
 * Specialization for the case where the transposition is not desired.
 * Represented by a -1.
 * In that case we can use a 2xB matrix.
 *
 * Time complexity:
 *     O(A * B)
 * Space:
 *     O(B)
 */
template <size_t inser_cost, size_t remov_cost, size_t subst_cost>
size_t vector_edit_distance<inser_cost, remov_cost, subst_cost, -1>
    ::distance(const string& a, const string& b) {
    size_t A = a.size(), B = b.size();

    vector<vector<size_t>> matrix(2, vector<size_t>(B + 1));
    
    vector<size_t>& previous = matrix[0];
    vector<size_t>& current = matrix[1];

    for (size_t j = 0; j <= B; ++j) {
        previous[j] = inser_cost * j;
    }

    for (size_t i = 1; i <= A; ++i) {
        current[0] = remov_cost * i;

        for (size_t j = 1; j <= B; ++j) {
            size_t dist_inser = current[j - 1] + inser_cost;
            size_t dist_remov = previous[j] + remov_cost;
            size_t dist_subst = previous[j - 1] + (a[i - 1] == b[j - 1] ? 0 : subst_cost);

            size_t best = std::min(dist_subst, std::min(dist_inser, dist_remov));

            // no transposition
            
            current[j] = best;
        }

        previous = matrix[i % 2];
        current = matrix[(i + 1) % 2];
    }

    return previous[B];
}

template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
class vector_edit_distance : public edit_distance {
public:
    size_t distance(const string& a, const string& b);
}

template <size_t inser_cost, size_t remov_cost, size_t subst_cost, size_t trans_cost>
size_t vector_edit_distance<inser_cost, remov_cost, subst_cost, trans_cost>
    ::distance(const string& a, const string& b) {
    size_t S1 = a.size(), S2 = b.size();

    vector<vector<size_t>> matrix(3, vector<size_t>(S2 + 1));

    for (size_t i = 0; i <= S1; ++i) {
    }

    for (size_t j = 0; j <= S2; ++j) {
        matrix[0][j] = inser_cost * j;
    }

    for (size_t i = 1; i <= S1; ++i) {

        matrix[i][0] = remov_cost * i;

        for (size_t j = 1; j <= S2; ++j) {
            size_t inser = matrix[i][j - 1] + inser_cost;
            size_t remov = matrix[i - 1][j] + remov_cost;
            size_t subst = matrix[i - 1][j - 1] + (a[i - 1] == b[j - 1]) ? 0 : subst_cost;

            size_t value = std::min(subst, std::min(inser, remov));

            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                size_t trans = matrix[i - 2][j - 2] + trans_cost;
                value = std::min(value, trans);
            }

            matrix[i][j] = value;
        }

        matrix[0] = matrix[1];
    }

    return matrix[S1][S2];
}
/**
 * Sellers minimum edit distance using a PxT matrix and without backtrace,
 * using Damerau-Levenshtein distance.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T * P)
 */
class matrix_sellers_damerau_distance : public string_distance {
private:
    enum class decision : char {
        match, substitution, insertion, removal, transposition
    };
public:
    size_t distance(const string& text, const string& pattern);
};

size_t matrix_sellers_damerau_distance::distance(const string& text, const string& pattern) {
    size_t P = pattern.size(), T = text.size();

    vector<vector<size_t>> matrix(P + 1, vector<size_t>(T + 1));
    
    for (size_t i = 0; i <= P; ++i) {
        matrix[i][0] = i;
    }

    for (size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0;
    }

    for (size_t i = 1; i <= P; ++i) {
        for (size_t j = 1; j <= T; ++j) {
            if (pattern[i - 1] == text[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            } else {
                size_t subst = matrix[i - 1][j - 1];
                size_t inser = matrix[i - 1][j];
                size_t remov = matrix[i][j - 1];

                size_t value = min(subst, min(inser, remov));

                if (i > 1 && j > 1 && pattern[i - 1] == text[j - 2] && pattern[i - 2] == text[j - 1]) {
                    size_t trans = matrix[0][j - 2];
                    value = min(value, trans);
                }

                matrix[i][j] = value + 1;
            }
        }
    }

    // Minimum of last row
    size_t minimum = *std::min_element(matrix[P].cbegin(), matrix[P].cend());
    return minimum;
}

/**
 * Sellers minimum edit distance using a 3xT matrix and without backtrace,
 * using Damerau-Levenshtein distance.
 *
 * Time complexity:
 *     O(T * P)
 * Space:
 *     O(T)
 */
class vector_sellers_damerau_distance : public string_distance {
public:
    size_t distance(const string& text, const string& pattern);
};

size_t vector_sellers_damerau_distance::distance(const string& text, const string& pattern) {
    size_t P = pattern.size(), T = text.size();

    vector<vector<size_t>> matrix(3, vector<size_t>(T + 1));

    for (size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0;
    }

    for (size_t i = 1; i <= P; ++i) {
        matrix[1][0] = i;

        for (size_t j = 1; j <= T; ++j) {
            if (pattern[i - 1] == text[j - 1]) {
                matrix[1][j] = matrix[i - 1][j - 1];
            } else {
                size_t subst = matrix[0][j - 1];
                size_t inser = matrix[0][j];
                size_t remov = matrix[1][j - 1];

                size_t value = min(subst, min(inser, remov));

                if (i > 1 && j > 1 && pattern[i - 1] == text[j - 2] && pattern[i - 2] == text[j - 1]) {
                    size_t trans = matrix[0][j - 2];
                    value = min(value, trans);
                }

                matrix[1][j] = value + 1;
            }
        }

        matrix[0] = matrix[1];
        matrix[1] = matrix[2];
    }

    size_t minimum = *std::min_element(matrix[1].cbegin(), matrix[1].cend());
    return minimum;
}

#endif

#endif // MIN_PRIORITY_QUEUE_H___
