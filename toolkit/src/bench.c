#include "bench.h"
#include <stdint.h>
#include <stdio.h>

uint64_t bench(bench_fn fn, void *ctx)
{
#if defined(__i386__) || defined(__x86_64__)

    uint64_t best = (uint64_t)-1;

    for (int i = 0; i < ITER; i++) {
        unsigned int lo1, hi1;
        unsigned int lo2, hi2;

        asm volatile("" ::: "memory");

        // --- START ---
        asm volatile(
            "cpuid\n"
            "rdtsc\n"
            : "=a"(lo1), "=d"(hi1)
            : "a"(0)
            : "rbx", "rcx"
        );

        fn(ctx);

        // --- END ---
        asm volatile(
            "rdtscp\n"
            : "=a"(lo2), "=d"(hi2)
            :
            : "rcx"
        );

        asm volatile("cpuid\n" ::: "rax", "rbx", "rcx", "rdx");

        uint64_t start = ((uint64_t)hi1 << 32) | lo1;
        uint64_t end   = ((uint64_t)hi2 << 32) | lo2;

        uint64_t delta = end - start;
        if (delta < best)
            best = delta;
    }

    return best;

#else
    // Fallback portable (aucune mesure cycle-accurate)
    for (int i = 0; i < ITER; i++)
        fn(ctx);
    return 0;
#endif
}

void work(void *ctx)
{
    int *x = (int *)ctx;
    *x += 1;
}


int main(void){
	int value = 0;
	uint64_t cycles = bench(work, &value);
	printf("%llu\n", (unsigned long long)cycles);
	return 0;
}