#ifndef BENCHMARK_H___
#define BENCHMARK_H___

#include <chrono>

using namespace std::literals::chrono_literals;
using micro_t = std::chrono::duration<unsigned long, std::micro>;
using now_t = std::chrono::time_point<std::chrono::steady_clock>;

now_t time_now();

micro_t time_diff(now_t start, now_t end);

#endif // BENCHMARK_H___
