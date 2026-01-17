#ifndef BENCH_H
#define BENCH_H

#include <stdint.h> //uint64_t

#define ITER 100

typedef void (*bench_fn)(void *ctx);

uint64_t bench(bench_fn fn, void *ctx);

#endif // BENCH_H