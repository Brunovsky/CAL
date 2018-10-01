/*
 * matcher.cpp
 */
#include <string>
#include <fstream>
#include <vector>

static std::vector<std::size_t> buildKMPtable(std::string W) {
	std::vector<int> T(W.size(), 0);

	int pos = 1, cnd = 0;
	T[0] = -1;

	while (pos < W.size()) {
		if (W[pos] == W[cnd]) {
			T[pos] = T[cnd];
		} else {
			T[pos] = cnd;

			cnd = T[cnd];

			while (cnd >= 0 && W[pos] != W[cnd]) {
				cnd = T[cnd];
			}
		}

		++pos; ++cnd;
	}

	return T;
}

int kmpMatcher(std::string S, std::string W) {
	int m = 0; // position in string S
	int i = 0; // position in pattern W
	auto T = buildKMPtable(W);

	int count = 0;

	while (m < S.size()) {
		if (W[i] == S[m + i]) {
			++i;
			if (i == W.size()) {
				++count;
			}
		} else {
			m -= T[i];
			i = T[i];
			if (i < 0) i = 0;
		}
	}
	return 0;
}

int numStringMatching(std::string filename, std::string toSearch) {
	// TODO

	std::ifstream file(filename);
	std::string s;

	int total_count = 0;

	while (getline(file, s)) {
		int line_count = 0;

		for (std::size_t i = 0; i < s.size() - toSearch.size() + 1; ++i) {
			if (s.substr(i, toSearch.size()) == toSearch)
				++line_count;
		}

		total_count += line_count;
	}

	return total_count;
}

int editDistance(std::string pattern, std::string text) {
	// TODO
	return 0;
}

float numApproximateStringMatching(std::string filename, std::string toSearch) {
	// TODO
	return 0;
}
