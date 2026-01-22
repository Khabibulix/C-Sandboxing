# include <stdio.h>
# include <stdint.h>
# include <string.h>

# define LINE_LEN 8
# define LONG_LINE_LEN 100
# define REALLY_LONG_LINE_LEN 10000
# define ITER 100

static char line[LINE_LEN + 1];
static char long_line[LONG_LINE_LEN + 1];
static char really_long_line[REALLY_LONG_LINE_LEN + 1];

struct test_case {
	char *buf;
	int len;
};

struct test_case cases[] = {
	{line, LINE_LEN},
	{long_line, LONG_LINE_LEN},
	{really_long_line, REALLY_LONG_LINE_LEN},
};


static inline void init_line(char *buf, int len){
	for (int i = 0; i < len; i++){
		buf[i] = 'a';
	buf[len] = 0;
	}
}

static inline unsigned long long rdtsc_start(void){
	unsigned int lo, hi;
	asm volatile(
	  "cpuid\n"
	  "rdtsc\n"
	  : "=a"(lo), "=d"(hi)
	  : "a"(0)
	  : "rbx", "rcx"
	);
	return ((unsigned long long)hi << 32) | lo;
}

static inline unsigned long long rdtsc_end(void){
	unsigned int lo, hi;
	asm volatile(
	  "rdtscp\n"
	  "mov %%eax, %0\n"
	  "mov %%edx, %1\n"
	  "cpuid\n"
	  : "=r"(lo), "=r"(hi)
	  :
	  : "rax", "rbx", "rcx", "rdx"
	);
	return ((unsigned long long)hi << 32) | lo;
}


unsigned long long bench_strlen(size_t (*func)(const char*),const char *line){
	unsigned long long best = (unsigned long long)-1;

	for (int i = 0; i < ITER; i++){
		asm volatile("" ::: "memory");
		unsigned long long start = rdtsc_start();
		volatile size_t r = func(line);
		unsigned long long end = rdtsc_end();

		unsigned long long delta = end - start;
		if(delta < best) best = delta;
	}
	return best;
}



static inline size_t my_strlen_word_wise(const char *s){
	const char *p = s;

	while (1) {
  		uint64_t word;
		memcpy(&word, p, sizeof(word));
		uint64_t tmp = (word - 0x0101010101010101ULL) &
			~word &
			0x8080808080808080ULL;

		if (tmp != 0){
			int byte_index = __builtin_ctzll(tmp) / 8;
			return (p - s) + byte_index;
		}

		p += 8;
	}
}

static inline size_t my_strlen(const char *s){
	const char *p = s;
	while (*p) p++;
	return p - s;
}


int main(void){

	for (int i = 0; i < 3; i++){
		init_line(cases[i].buf, cases[i].len);
	}

	printf("%-12s %-8s %-8s\n", "Function", "Len", "Cycles");
	printf("----------------------------------\n");
	printf("%-12s %-8d %-8llu\n", "my_strlen", LINE_LEN, bench_strlen(my_strlen, line));
	printf("%-12s %-8d %-8llu\n", "glibc", LINE_LEN, bench_strlen(strlen, line));
	printf("%-12s %-8d %-8llu\n", "my_strlenww", LINE_LEN, bench_strlen(my_strlen_word_wise, line));
	printf("----------------------------------\n");

	printf("%-12s %-8d %-8llu\n", "my_strlen", LONG_LINE_LEN, bench_strlen(my_strlen, long_line));
	printf("%-12s %-8d %-8llu\n", "glibc", LONG_LINE_LEN, bench_strlen(strlen, long_line));
	printf("%-12s %-8d %-8llu\n", "my_strlenww", LONG_LINE_LEN, bench_strlen(my_strlen_word_wise, long_line));
	printf("----------------------------------\n");

	printf("%-12s %-8d %-8llu\n", "my_strlen", REALLY_LONG_LINE_LEN, bench_strlen(my_strlen, really_long_line));
	printf("%-12s %-8d %-8llu\n", "glibc", REALLY_LONG_LINE_LEN, bench_strlen(strlen, really_long_line));
	printf("%-12s %-8d %-8llu\n", "my_strlenww", REALLY_LONG_LINE_LEN, bench_strlen(my_strlen_word_wise, really_long_line));
	

}
