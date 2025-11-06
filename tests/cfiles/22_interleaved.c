/*
 * Copyright (c) Oliver Krisetya, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64
#define NLINES 3

struct cache_line {
    long a;
    char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line lines[NLINES];

int 
main()
{
    // Perform several writes to different offsets within the block
    for (int i = 0; i < NLINES; i++)
    {
        lines[i].a = i * 10;
    }

    // Read back the values to ensure they are updated (should be hits)
    int sum = 0;
    for (int i = 0; i < NLINES; i++)
    {
        sum += lines[i].a;
    }
    return sum;
}
