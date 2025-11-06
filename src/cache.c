#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h" 


struct cache_line {
	int valid;          // Indicating if the line holds valid data (1) or not (0)
	unsigned long tag;  // Stores the tag part of the address
};

// Main cache structure
struct cache {
	struct cache_line *lines; // An array where each element represents a cache set/line
	unsigned long num_sets;   // The total number of sets (and lines) in the cache

	// Statistics counters
	unsigned long num_cache_accesses; // Total number of times cache_access was called
	unsigned long num_cache_misses;   // Total number of cache misses occurred
};


struct cache *cache_allocate(unsigned long num_cache_lines, unsigned long num_sets) {
	if (num_cache_lines != num_sets) {
		fprintf(stderr, "Error: For direct-mapped cache simulation, num_cache_lines (%lu) must equal num_sets (%lu).\n",
				num_cache_lines, num_sets);
	}
	// Cache must have at least one set/line.
	if (num_sets == 0) {
		fprintf(stderr, "Error: Cache must have at least one set (num_sets cannot be 0).\n");
		return NULL; // Indicate failure
	}


	// Allocate memory for the main cache structure itself
	struct cache *c = (struct cache *)malloc(sizeof(struct cache));
	if (c == NULL) {
		perror("Failed to allocate cache structure");
		return NULL; // Allocation failed
	}

	// Initialize the cache properties and statistics
	c->num_sets = num_sets;
	c->num_cache_accesses = 0;
	c->num_cache_misses = 0;

	// Allocate memory for the array of cache lines (one per set)
	// Use calloc to allocate and zero-initialize the memory. This sets
	// valid = 0 and tag = 0 for all lines initially.
	c->lines = (struct cache_line *)calloc(num_sets, sizeof(struct cache_line));
	if (c->lines == NULL) {
		perror("Failed to allocate cache lines array");
		free(c); // Clean up the partially allocated cache structure
		return NULL; // Allocation failed
	}

	return c; // Return the pointer to the successfully allocated and initialized cache
}


void cache_free(struct cache *c) {
	if (c != NULL) {
		// Free the array of cache lines first, if it was allocated
		if (c->lines != NULL) {
			free(c->lines);
			c->lines = NULL; 
		}
		// Then free the cache structure itself
		free(c);
	}
}


void cache_access(struct cache *c, unsigned long mem_addr) {
	// Basic validation
	if (c == NULL || c->lines == NULL) {
		fprintf(stderr, "Error: cache_access called on an uninitialized or invalid cache.\n");
		return; 
	}

	// Increment the total number of cache accesses for statistics
	c->num_cache_accesses++;

	// Cache line size is fixed at 64 bytes
	const unsigned long block_size = 64;

	// Calculate the block address
	unsigned long block_addr_tag = mem_addr / block_size;

	// Calculate the index of the set where this address maps.
	unsigned long set_index = block_addr_tag % c->num_sets;

	// Get a pointer to the specific cache line corresponding to the calculated set index
	struct cache_line *line = &c->lines[set_index];

	// Check if the line is valid AND if the tag matches the current access's tag
	if (line->valid && line->tag == block_addr_tag) {
		// Cache Hit
	} else {
		// Cache Miss

		// Increment the miss counter
		c->num_cache_misses++;

		// Bring the new block into the cache:
		// Update the line's tag and mark it as valid.
		line->valid = 1;
		line->tag = block_addr_tag;
	}
}


struct cache_stats cache_statistics(struct cache *c) {
	// Handle case where cache might be NULL 
	if (c == NULL) {
		// Return zeroed statistics as a default
		return (struct cache_stats){ .num_cache_accesses = 0, .num_cache_misses = 0 };
	}

	struct cache_stats stats;
	stats.num_cache_accesses = c->num_cache_accesses;
	stats.num_cache_misses = c->num_cache_misses;
	return stats;
}