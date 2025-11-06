/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64
#define NLINES 8
#define MULT (NLINES*BLOCK_SZ)

struct cache_line {
    long a;
    char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line lines[NLINES]; 

int 
main()
{ 
    char * p1 = (char *)&lines[0];
    *(short*)p1 = 1;
    char * p2 = p1 + MULT;
    *(short*)p2 = 2;
    char * p3 = p2 + MULT*2;
    *(short*)p3 = 3;

    return;
}
