/*
 * Copyright (c) Oliver Krisetya, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64    
#define NLINES 4

struct cache_line {
	long a;
	char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));


struct cache_line lines[NLINES];

int main()
{
    int i;

    // Access one element per block in a round-robin fashion.
    for (i = 0; i < NLINES; i++)
    {
        lines[i].a = i;
    }
    return;
}
