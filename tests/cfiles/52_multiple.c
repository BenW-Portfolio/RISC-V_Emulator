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
	long a;
	char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line lines[NLINES];

int
main(void)
{
	int i, j;

	for (j = 0; j < NACCESSES; j++) {
		for (i = 0; i < NLINES; i++) {
			lines[i].a = 42;
		}
	}

	return 0;
}
