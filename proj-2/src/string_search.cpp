#include "string_search.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <iomanip>

#define ASCII_SIZE 256

namespace search {

std::vector<std::size_t> naive_search(const std::string& text, const std::string& pattern) {
    std::size_t T = text.size(), P = pattern.size();
    std::vector<std::size_t> match;

    for (std::size_t i = 0; i + P <= T; ++i) {
        bool success = true;

        for (std::size_t j = 0; j < P; ++j) {
            if (text[i + j] != pattern[j]) {
                success = false;
                break;
            } 
        }

        if (success) {
            match.push_back(i);
        }
    }

    return match;
}





bad_char_table_rule::bad_char_table_rule(const std::string& pattern) {
    std::size_t P = pattern.size();

    matrix.resize(ASCII_SIZE, std::vector<std::size_t>(P, -1));
    
    for (std::size_t index = 0; index < P; ++index) {
        std::size_t c = static_cast<unsigned char>(pattern[index]);

        for (std::size_t j = index + 1; j < P; ++j) {
            matrix[c][j] = index;
        }
    }
}

std::size_t bad_char_table_rule::shift(std::size_t index, char text_char) const {
    std::size_t c = static_cast<unsigned char>(text_char);
    if (c >= matrix.size()) {
        return index + 1;
    } else {
        return index - matrix[c][index];
    }
}

bad_char_map_rule::bad_char_map_rule(const std::string& pattern) {
    std::size_t P = pattern.size();

    mapping.resize(ASCII_SIZE, std::string::npos);

    std::size_t count = 0;

    for (std::size_t index = 0; index < P; ++index) {
        std::size_t c = static_cast<unsigned char>(pattern[index]);

        // This is the first c in pattern
        if (mapping[c] == std::string::npos) {
            mapping[c] = count++;
            matrix.push_back(std::vector<std::size_t>(P, -1));
        }

        for (std::size_t j = index + 1; j < P; ++j) {
            matrix[mapping[c]][j] = index;
        }
    }
}

std::size_t bad_char_map_rule::shift(std::size_t index, char text_char) const {
    std::size_t c = text_char;
    if (c >= mapping.size() || mapping[c] == std::string::npos) {
        return index + 1;
    } else {
        return index - matrix[mapping[c]][index];
    }
}

bad_char_list_rule::bad_char_list_rule(const std::string& pattern) {
    std::size_t P = pattern.size();

    list.resize(ASCII_SIZE, -1);

    for (std::size_t i = 0; i < P; ++i) {
        std::size_t c = static_cast<unsigned char>(pattern[i]);
        list[c] = i;
    }
}

std::size_t bad_char_list_rule::shift(std::size_t index, char text_char) const {
    std::size_t c = static_cast<unsigned char>(text_char);
    if (c >= list.size()) {
        return index + 1;
    } else if (list[c] != std::string::npos && list[c] > index) {
        return 1;
    } else {
        return index - list[c];
    }
}

good_suffix_rule::good_suffix_rule(const std::string& pattern) {
    std::size_t P = pattern.size();
    lookup.resize(P + 1);

    std::size_t index = P, border = P + 1;
    std::vector<std::size_t> f(P + 1);

    for (f[index] = border; index > 0; f[--index] = --border) {
        while (border <= P && pattern[index - 1] != pattern[border - 1]) {
            if (lookup[border] == 0) lookup[border] = border - index;
            border = f[border]; // recurse inside the right side of the border
        }
    }

    for (index = 0, border = f[0]; index <= P; ++index) {
        if (lookup[index] == 0) lookup[index] = border;
        if (index == border) border = f[border];
    }
}

std::size_t good_suffix_rule::shift(std::size_t index) const {
    if (index >= lookup.size()) {
        return index + 1;
    } else {
        return lookup[index];
    }
}





knuth_morris_pratt::knuth_morris_pratt(std::string pattern): pattern(pattern) {
    std::size_t P = pattern.size();

    lookup_.resize(P + 1);

    lookup_[0] = -1;
    long border = 0; // need to compare against 0
    
    for (std::size_t index = 1; index < P; ++index, ++border) {
        if (pattern[index] == pattern[border]) {
            lookup_[index] = lookup_[border];
        } else {
            lookup_[index] = border;

            do {
                border = lookup_[border];
            } while (border >= 0 && pattern[index] != pattern[border]);
        }
    }

    lookup_[P] = border;
}

int knuth_morris_pratt::lookup(std::size_t index) const {
    return lookup_[index];
}

std::size_t knuth_morris_pratt::shift(std::size_t index) const {
    return index - lookup_[index];
}

const std::string& knuth_morris_pratt::get_pattern() const {
    return pattern;
}

std::vector<std::size_t> knuth_morris_pratt_search(const std::string& text, const knuth_morris_pratt& kmp) {
    const std::string &pattern = kmp.get_pattern();
    long P = pattern.size(), T = text.size();

    std::vector<std::size_t> match;
    if (P == 0) return match;

    long i = 0;
    long j = 0;

    while (i <= T - P) {
        while ((j < P) && (text[i + j] == pattern[j])) ++j;

        if (j == P) {
            // Matched
            match.push_back(i);
            i += kmp.shift(P);
            j = kmp.lookup(P);
        } else {
            // Mismatched
            i += kmp.shift(j);
            j = kmp.lookup(j);
        }

        if (j < 0) j = 0;
    }

    return match;
}





std::size_t hamming_distance(const std::string& str1, const std::string& str2) {
    std::size_t S1 = str1.size(), S2 = str2.size();

    assert(S1 == S2 && "Bad hamming_distance call");

    std::size_t count = 0;
    for (std::size_t i = 0; i < S1; ++i) {
        if (str1[i] != str2[i]) ++count;
    }

    return count;
}

std::size_t matrix_levenshtein_distance(const std::string& a, const std::string& b) {
    std::size_t A = a.size(), B = b.size();

    std::vector<std::vector<std::size_t>> matrix(A + 1, std::vector<std::size_t>(B + 1));

    for (std::size_t i = 0; i <= A; ++i) {
        matrix[i][0] = i;
    }

    for (std::size_t j = 0; j <= B; ++j) {
        matrix[0][j] = j;
    }

    for (std::size_t i = 1; i <= A; ++i) {
        for (std::size_t j = 1; j <= B; ++j) {
            std::size_t s = a[i - 1] == b[j - 1] ? 0 : 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            matrix[i][j] = value;
        }
    }

    return matrix[A][B];
}

std::size_t vector_levenshtein_distance(const std::string& a, const std::string& b) {
    // Ensure b is the smallest string.
    if (a.size() < b.size()) return vector_levenshtein_distance(b, a);

    std::size_t A = a.size(), B = b.size();

    std::vector<std::vector<std::size_t>> matrix(2, std::vector<std::size_t>(B + 1));

    for (std::size_t j = 0; j <= B; ++j) {
        matrix[0][j] = j;
    }

    for (std::size_t i = 1; i <= A; ++i) {
        auto& current_row = matrix[i % 2];
        auto& previous_row = matrix[(i + 1) % 2];

        current_row[0] = i;

        for (std::size_t j = 1; j <= B; ++j) {
            std::size_t s = a[i - 1] == b[j - 1] ? 0 : 1;

            std::size_t subst = s + previous_row[j - 1];
            std::size_t inser = 1 + previous_row[j];
            std::size_t remov = 1 + current_row[j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            current_row[j] = value;
        }
    }

    return matrix[A % 2][B];
}

std::size_t levenshtein_distance(const std::string& a, const std::string& b) {
    // Redirect
    return vector_levenshtein_distance(a, b);
}

std::size_t matrix_restricted_damerau_distance(const std::string& a, const std::string& b) {
    std::size_t A = a.size(), B = b.size();

    std::vector<std::vector<std::size_t>> matrix(A + 1, std::vector<std::size_t>(B + 1));

    for (std::size_t i = 0; i <= A; ++i) {
        matrix[i][0] = i;
    }

    for (std::size_t j = 0; j <= B; ++j) {
        matrix[0][j] = j;
    }

    for (std::size_t i = 1; i <= A; ++i) {
        for (std::size_t j = 1; j <= B; ++j) {
            std::size_t s = a[i - 1] == b[j - 1] ? 0 : 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                std::size_t trans = 1 + matrix[i - 2][j - 2];
                value = std::min(value, trans);
            }

            matrix[i][j] = value;
        }
    }

    return matrix[A][B];
}

std::size_t vector_restricted_damerau_distance(const std::string& a, const std::string& b) {
    std::size_t A = a.size(), B = b.size();

    std::vector<std::vector<std::size_t>> matrix(3, std::vector<std::size_t>(B + 1));

    for (std::size_t j = 0; j <= B; ++j) {
        matrix[0][j] = j;
    }

    for (std::size_t i = 1; i <= A; ++i) {
        auto& current_row = matrix[i % 3];
        auto& previous_row = matrix[(i - 1) % 3];
        auto& old_row = matrix[(i + 1) % 3];

        current_row[0] = i;

        for (std::size_t j = 1; j <= B; ++j) {
            std::size_t s = a[i - 1] == b[j - 1] ? 0 : 1;

            std::size_t subst = s + previous_row[j - 1];
            std::size_t inser = 1 + previous_row[j];
            std::size_t remov = 1 + current_row[j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                std::size_t trans = 1 + old_row[j - 2];
                value = std::min(value, trans);
            }

            current_row[j] = value;
        }
    }

    return matrix[A % 3][B];
}

std::size_t restricted_damerau_distance(const std::string& a, const std::string& b) {
    // Redirect
    return vector_restricted_damerau_distance(a, b);
}

std::size_t damerau_distance(const std::string& a, const std::string& b) {
    std::size_t A = a.size(), B = b.size();

    std::vector<std::vector<std::size_t>> matrix(A + 2, std::vector<std::size_t>(B + 2));

    for (std::size_t i = 0; i <= A + 1; ++i) {
        matrix[i][0] = A + B;
        matrix[i][1] = i - 1;
    }

    for (std::size_t j = 0; j <= B + 1; ++j) {
        matrix[0][j] = A + B;
        matrix[1][j] = j - 1;
    }

    matrix[1][0] = A + B;

    std::vector<std::size_t> occurrence(ASCII_SIZE, 1);

    for (std::size_t i = 2; i <= A + 1; ++i) {
        // the previous j where a match a[i-2] = b[j-2] occurred.
        std::size_t previous_j = 1;

        std::size_t ai = static_cast<unsigned char>(a[i - 2]);

        for (std::size_t j = 2; j <= B + 1; ++j) {
            std::size_t bj = static_cast<unsigned char>(b[j - 2]);

            // the previous i where a[i-2] == b[j-2]
            std::size_t previous_i = occurrence[bj];

            // cost of substitution
            std::size_t s = a[i - 2] == b[j - 2] ? 0 : 1;

            // cost of transposition:
            // i - previous_i - 1 insertions
            // j - previous_j - 1 deletions
            // 1 transposition
            std::size_t t = (i - previous_i) + (j - previous_j) - 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];
            std::size_t trans = t + matrix[previous_i - 1][previous_j - 1];

            std::size_t value = std::min(std::min(subst, trans), std::min(inser, remov));

            matrix[i][j] = value;

            if (!s) previous_j = j;
        }

        occurrence[ai] = i;
    }

    return matrix[A + 1][B + 1];
}





std::size_t matrix_levenshtein_fuzzy(const std::string& text, const std::string& pattern) {
    std::size_t P = pattern.size(), T = text.size();

    std::vector<std::vector<std::size_t>> matrix(P + 1, std::vector<std::size_t>(T + 1));
    
    for (std::size_t i = 0; i <= P; ++i) {
        matrix[i][0] = i;
    }

    for (std::size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0; // ! FUZZY
    }

    for (std::size_t i = 1; i <= P; ++i) {
        for (std::size_t j = 1; j <= T; ++j) {
            std::size_t s = pattern[i - 1] == text[j - 1] ? 0 : 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            matrix[i][j] = value;
        }
    }

    // ! FUZZY: Minimum of last row
    std::size_t minimum = *std::min_element(matrix[P].cbegin(), matrix[P].cend());
    return minimum;
}

std::size_t vector_levenshtein_fuzzy(const std::string& text, const std::string& pattern) {
    std::size_t P = pattern.size(), T = text.size();

    std::vector<std::vector<std::size_t>> matrix(2, std::vector<std::size_t>(T + 1));

    for (std::size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0; // ! FUZZY
    }

    for (std::size_t i = 1; i <= P; ++i) {
        auto& current_row = matrix[i % 2];
        auto& previous_row = matrix[(i + 1) % 2];

        current_row[0] = i;

        for (std::size_t j = 1; j <= T; ++j) {
            std::size_t s = pattern[i - 1] == text[j - 1] ? 0 : 1;

            std::size_t subst = s + previous_row[j - 1];
            std::size_t inser = 1 + previous_row[j];
            std::size_t remov = 1 + current_row[j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            current_row[j] = value;
        }
    }

    // ! FUZZY: Minimum of last row
    std::size_t minimum = *std::min_element(matrix[P % 2].cbegin(), matrix[P % 2].cend());
    return minimum;
}

std::size_t levenshtein_fuzzy(const std::string& text, const std::string& pattern) {
    // Redirect
    return vector_levenshtein_fuzzy(text, pattern);
}

std::size_t matrix_restricted_damerau_fuzzy(const std::string& text, const std::string& pattern) {
    std::size_t P = pattern.size(), T = text.size();

    std::vector<std::vector<std::size_t>> matrix(P + 1, std::vector<std::size_t>(T + 1));
    
    for (std::size_t i = 0; i <= P; ++i) {
        matrix[i][0] = i;
    }

    for (std::size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0; // ! FUZZY
    }

    for (std::size_t i = 1; i <= P; ++i) {
        for (std::size_t j = 1; j <= T; ++j) {
            std::size_t s = pattern[i - 1] == text[j - 1] ? 0 : 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            if (i > 1 && j > 1 && pattern[i - 1] == text[j - 2] && pattern[i - 2] == text[j - 1]) {
                std::size_t trans = 1 + matrix[i - 2][j - 2];
                value = std::min(value, trans);
            }

            matrix[i][j] = value;
        }
    }

    // ! FUZZY: Minimum of last row
    std::size_t minimum = *std::min_element(matrix[P].cbegin(), matrix[P].cend());
    return minimum;
}

std::size_t vector_restricted_damerau_fuzzy(const std::string& text, const std::string& pattern) {
    std::size_t P = pattern.size(), T = text.size();

    std::vector<std::vector<std::size_t>> matrix(3, std::vector<std::size_t>(T + 1));

    for (std::size_t j = 0; j <= T; ++j) {
        matrix[0][j] = 0; // ! FUZZY
    }

    for (std::size_t i = 1; i <= P; ++i) {
        auto& current_row = matrix[i % 3];
        auto& previous_row = matrix[(i - 1) % 3];
        auto& old_row = matrix[(i + 1) % 3];

        current_row[0] = i;

        for (std::size_t j = 1; j <= T; ++j) {
            std::size_t s = pattern[i - 1] == text[j - 1] ? 0 : 1;

            std::size_t subst = s + previous_row[j - 1];
            std::size_t inser = 1 + previous_row[j];
            std::size_t remov = 1 + current_row[j - 1];

            std::size_t value = std::min(subst, std::min(inser, remov));

            if (i > 1 && j > 1 && pattern[i - 1] == text[j - 2] && pattern[i - 2] == text[j - 1]) {
                std::size_t trans = 1 + old_row[j - 2];
                value = std::min(value, trans);
            }

            current_row[j] = value;
        }
    }

    // ! FUZZY: Minimum of last row
    std::size_t minimum = *std::min_element(matrix[P % 3].cbegin(), matrix[P % 3].cend());
    return minimum;
}

std::size_t restricted_damerau_fuzzy(const std::string& text, const std::string& pattern) {
    // Restricted
    return vector_restricted_damerau_fuzzy(text, pattern);
}

std::size_t damerau_fuzzy(const std::string& text, const std::string& pattern) {
    std::size_t P = pattern.size(), T = text.size();

    std::vector<std::vector<std::size_t>> matrix(P + 2, std::vector<std::size_t>(T + 2));

    for (std::size_t i = 0; i <= P + 1; ++i) {
        matrix[i][0] = P + T;
        matrix[i][1] = i - 1;
    }

    for (std::size_t j = 0; j <= T + 1; ++j) {
        matrix[0][j] = P + T;
        matrix[1][j] = 0; // ! FUZZY
    }

    matrix[1][0] = P + T;

    std::vector<std::size_t> occurrence(ASCII_SIZE, 1);

    for (std::size_t i = 2; i <= P + 1; ++i) {
        // the previous j where a match pattern[i-2] = text[j-2] occurred.
        std::size_t previous_j = 1;

        std::size_t patterni = static_cast<unsigned char>(pattern[i - 2]);

        for (std::size_t j = 2; j <= T + 1; ++j) {
            std::size_t textj = static_cast<unsigned char>(text[j - 2]);

            // the previous i where pattern[i-2] == text[j-2]
            std::size_t previous_i = occurrence[textj];

            // cost of substitution
            std::size_t s = pattern[i - 2] == text[j - 2] ? 0 : 1;

            // cost of transposition:
            // i - previous_i - 1 insertions
            // j - previous_j - 1 deletions
            // 1 transposition
            std::size_t t = (i - previous_i) + (j - previous_j) - 1;

            std::size_t subst = s + matrix[i - 1][j - 1];
            std::size_t inser = 1 + matrix[i - 1][j];
            std::size_t remov = 1 + matrix[i][j - 1];
            std::size_t trans = t + matrix[previous_i - 1][previous_j - 1];

            std::size_t value = std::min(std::min(subst, trans), std::min(inser, remov));

            matrix[i][j] = value;

            if (!s) previous_j = j;
        }

        occurrence[patterni] = i;
    }

    // ! FUZZY: Minimum of last row
    std::size_t minimum = *std::min_element(matrix[P + 1].cbegin(), matrix[P + 1].cend());
    return minimum;
}

} // namespace search
