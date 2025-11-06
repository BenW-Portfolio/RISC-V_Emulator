/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

#define BLOCK_SZ 64

struct cache_line {
    int a[BLOCK_SZ / sizeof(int)];
} __attribute__((aligned(BLOCK_SZ)));

struct cache_line line; 

int 
main()
{ 
    line.a[0] = 1;
    line.a[1] = 2;
    line.a[2] = 3;

    return;
}
