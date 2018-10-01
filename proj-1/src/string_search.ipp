/**
 * Template implementation of string_search.h Boyer Moore
 */
#include <algorithm>

namespace search {

template <typename bad_t, typename good_t>
boyer_moore<bad_t, good_t>::boyer_moore(std::string pattern):
    bad(pattern), good(pattern), pattern(pattern) {}

template <typename bad_t, typename good_t>
std::size_t boyer_moore<bad_t, good_t>::bad_shift(std::size_t index, char text_char) const {
    return bad.shift(index, text_char);
}

template <typename bad_t, typename good_t>
std::size_t boyer_moore<bad_t, good_t>::good_shift(std::size_t index) const {
    return good.shift(index);
}

template <typename bad_t, typename good_t>
std::size_t boyer_moore<bad_t, good_t>::mismatch_shift(std::size_t mismatch_index, char text_char) const {
    return std::max(bad.shift(mismatch_index, text_char), good.shift(mismatch_index + 1));
}

template <typename bad_t, typename good_t>
std::size_t boyer_moore<bad_t, good_t>::match_shift() const {
    return good.shift(0);
}

template <typename bad_t, typename good_t>
const std::string& boyer_moore<bad_t, good_t>::get_pattern() const {
    return pattern;
}

template <typename bad_t, typename good_t>
std::vector<std::size_t> boyer_moore_search(const std::string& text, const boyer_moore<bad_t, good_t>& bm) {
    const std::string& pattern = bm.get_pattern();
    long P = pattern.size(), T = text.size();

    //std::cout << "text: " << text << std::endl;
    //std::cout << "pattern: " << pattern << std::endl;

    std::vector<std::size_t> match;
    if (P == 0) return match;

    long i = 0;
    long j = P - 1;
    
    while (i <= T - P) {
        while ((j >= 0) && (text[i + j] == pattern[j])) {
            //std::cout << "Match at j = " << j << std::endl;
            --j;
        }

        if (j < 0) {
            // Matched
            //std::cout << "Match at i = " << i << std::endl;
            match.push_back(i);
            i += bm.match_shift();
            //std::cout << "New i = " << i << std::endl;
        } else {
            // Mismatched
            //std::cout << "Mismatch at i = " << i << std::endl;
            i += bm.mismatch_shift(j, text[i + j]);
            //std::cout << "New i = " << i << std::endl;
        }

        j = P - 1;
        //std::cout << "i,j = " << i << "," << j << std::endl;
    }

    return match;
}

template <typename bad_t, typename good_t>
std::vector<std::size_t> boyer_moore_galil_search(const std::string& text, const boyer_moore<bad_t, good_t>& bm) {
    const std::string& pattern = bm.get_pattern();
    long P = pattern.size(), T = text.size();

    std::vector<std::size_t> match;
    if (P == 0) return match;

    long i = 0;
    long j = P - 1;
    long galil = 0;
    
    while (i <= T - P) {
        while ((j >= galil) && (text[i + j] == pattern[j])) {
            --j;
        }

        // NOTE: INCOMPLETE

        if (j < galil) {
            // Matched
            match.push_back(i);
            galil = P - 1 - bm.match_shift();
            i += bm.match_shift();
        } else {
            // Mismatched
            galil = 0;
            i += bm.mismatch_shift(j, text[i + j]);
        }

        j = P - 1;
    }

    return match;
}

}
