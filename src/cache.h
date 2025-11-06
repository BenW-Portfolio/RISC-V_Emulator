#pragma once

/* DO NOT MODIFY THIS FILE */

/* Define this as you'd like in your .c file. */
struct cache;

/* 
 * Emulate the behavior of a cache by tracking cache accesses and misses. You do not need to store values
 * in the cache for this assignment, just the addresses. This is a simplified cache implementation.
 * 
 * A full implementation would require storing both the addresses and their values, similar to 
 * a key-value store. If data is already in the cache, you should not need to load from memory.
 * Our simplified implementation will store all data in memory and only track the addresses in the cache.
 * 
 * @param c The cache structure to operate on.
 * @param mem_addr The memory address to access in the cache.
 */
void cache_access(struct cache *c, unsigned long mem_addr);

struct cache_stats {
	unsigned long num_cache_accesses, num_cache_misses;
};
struct cache_stats cache_statistics(struct cache *c);

struct cache *cache_allocate(unsigned long num_cache_lines, unsigned long num_sets);
void cache_free(struct cache *c);
