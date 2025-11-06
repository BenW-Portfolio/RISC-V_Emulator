/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64
#define NLINES 4
struct cache_line {
    long a;
    char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line lines[NLINES];

int 
main()
{ 
    lines[0].a = 8;
    lines[1].a = 42;
    lines[3].a = 68;
    
    return;
}
