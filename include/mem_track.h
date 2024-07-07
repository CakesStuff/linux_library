#ifndef _MEM_TRACK_H
#define _MEM_TRACK_H
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

/**
 * A tracked version of malloc for memory analysis.
 * 
 * \param size the amount of memory to allocate
 * \param line the line in which it was called
 * \param func the function from which it was called
 * \param file the file from which it was called
 * \param permanent whether or not the allocation is permanent (forever)
 * \returns A pointer to the allocated memory.
 */
void* tracked_malloc(size_t size, size_t line, const char* func, char* file, bool permanent);
/**
 * A tracked version of realloc for memory analysis.
 * 
 * \param arr the array to reallocate
 * \param size the new amount of memory to allocate
 * \param line the line in which it was called
 * \param func the function from which it was called
 * \param file the file from which it was called
 * \returns A pointer to the allocated memory.
 */
void* tracked_realloc(void* arr, size_t size, size_t line, const char* func, char* file);
/**
 * A tracked version of free for memory analysis.
 * 
 * \param arr the array to free
 * \param line the line in which it was called
 * \param func the function from which it was called
 * \param file the file from which it was called
 * \returns A pointer to the allocated memory.
 */
void tracked_free(void* arr, size_t line, const char* func, char* file);

#define TRACKED_MALLOC(size) tracked_malloc(size, __LINE__, __func__, __FILE__, false)
#define TRACKED_STATIC_MALLOC(size) tracked_malloc(size, __LINE__, __func__, __FILE__, true)
#define TRACKED_REALLOC(arr, size) tracked_realloc(arr, size, __LINE__, __func__, __FILE__)
#define TRACKED_FREE(arr) tracked_free(arr, __LINE__, __func__, __FILE__)

#define malloc TRACKED_MALLOC
#define realloc TRACKED_REALLOC
#define free TRACKED_FREE

/**
 * Gets the currently tracked memory usage.
 * Does not consider alignment or how much memory is actually allocated.
 * 
 * \returns The amount.
 */
size_t tracked_memory_usage(void);

/**
 * Prints the currently tracked memory usage and non freed dynamic allocations to file.
 * 
 * \param file the file to print to
 */
void print_memory_trace(FILE* file);

#endif /* _MEM_TRACK_H */