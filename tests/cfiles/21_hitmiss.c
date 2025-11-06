/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64

struct cache_line {
    long a;
    char padding[BLOCK_SZ - sizeof(long)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line line;

int 
main()
{ 
    line.a = 1;
    line.a = 2;
    line.a = 3;
    
    return;
}
