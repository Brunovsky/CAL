#include "benchmark.h"

now_t time_now() {
    return std::chrono::steady_clock::now();
}

micro_t time_diff(now_t start, now_t end) {
    return std::chrono::duration_cast<micro_t>(end - start);
}
