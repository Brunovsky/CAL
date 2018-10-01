#include "algo.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

namespace {

template <typename T>
void print_vector(const std::vector<T>& vec, int width = 3) {
  for (std::size_t i = 0; i < vec.size(); ++i) {
    std::cout << std::setw(width) << vec[i] << " ";
  }
  std::cout << std::endl;
}

}

std::vector<int> longest_increasing_subsequence(std::vector<int> seq) {
  std::vector<std::size_t> best(seq.size());
  std::vector<std::size_t> pred(seq.size());

  std::size_t L = 0;

  const auto show = [&]() {
    if (L == 0) return;

    std::vector<int> sofar;
    std::size_t k = best[L - 1];

    for (std::size_t i = 0; i < L; ++i) {
      sofar.push_back(seq[k]);
      k = pred[k];
    }

    std::reverse(sofar.begin(), sofar.end());
    print_vector(best);
    print_vector(pred);
    print_vector(sofar);
    std::cout << std::endl;
  };

  for (std::size_t i = 0; i < seq.size(); ++i) {
    std::size_t j = L;
    while (j-- > 0) {
      if (seq[best[j]] < seq[i]) {
        best[j + 1] = i;
        pred[i] = best[j];

        if (j + 1 == L) ++L;
        break;
      }
    }
    if (++j == 0) {
      best[0] = i;
      pred[i] = 0;

      if (L == 0) ++L;
    }
    show();
  }

  return std::vector<int>();
}
