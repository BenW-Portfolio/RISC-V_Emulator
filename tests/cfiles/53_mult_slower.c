/*
 * Copyright (c) Gabriel Parmer, 2024.
 * Do not copy nor distribute without permission.
 */

/*
 * Run this twice: once with cache < 8 cache-lines, and another with
 * a larger cache.
 */

#define BLOCK_SZ 64
#define NLINES 8
#define NACCESSES 3

struct cache_line {
	char a[BLOCK_SZ];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line lines[NLINES];

int
main(void)
{
	int i, j, k;

	for (k = 0; k < NACCESSES; k++) {
		for (i = 0; i < NLINES; i++) {
			/* 
			 * Instead of storing a single value, since cache lines are 64 bytes in 
			 * size, we will store 64 values each line. 
			 * We will get more hits here.
			 */
			for (j = 0; j < 64; j++) {
				lines[i].a[j] = 42;
			}
		}
	}

	return 0;
}
