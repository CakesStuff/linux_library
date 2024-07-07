#ifndef _MEM_TRACK_H
#include<mem_track.h> // Defines _MEM_TRACK_H

#undef malloc
#undef realloc
#undef free

#include<internal/core.h>

#define FAULTY_FLAG  0b00001000
#define MALLOC_FLAG  0b00000100
#define REALLOC_FLAG 0b00000010
#define STATIC_FLAG  0b00000001

/**
 * Data for a single allocation.
 */
typedef struct {
    void* ptr;
    const char* func,* file;
    size_t line, size;
    byte flags;
} allocation;

/**
 * Vector of allocation for memory tracking.
 */
DEFINE_VECTOR_OF(allocation) allocation_v;

/**
 * Type for global data.
 */
typedef struct {
    allocation_v allocations;
    bool initialized;
} mem_track_g;

/**
 * Initializes global memory.
 */
void mem_track_init(void);

/**
 * Finds an allocation in the global allocation vector.
 * 
 * \param allocation the allocation to find
 * \returns The index of the allocation or SIZE_MAX if it was not found.
 */
size_t find_allocation(void* allocation);

/**
 * The global data.
 */
extern mem_track_g mtg;

#endif /* _MEM_TRACK_H */