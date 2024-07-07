#include<internal/mem_track.h>

void mem_track_init()
{
    VECTOR_INIT(mtg.allocations);

    mtg.initialized = true;
}

size_t find_allocation(void* allocation)
{
    for(size_t i = 0; i < mtg.allocations.size; i++)
    {
        if(mtg.allocations.arr[i].ptr == allocation)
        {
            return i;
        }
    }

    return SIZE_MAX;
}

void* tracked_malloc(size_t size, size_t line, const char* func, char* file, bool permanent)
{
    if(!mtg.initialized)mem_track_init();

    allocation a = {
        .file = file,
        .func = func,
        .line = line,
        .size = size,
        .flags = MALLOC_FLAG | (permanent ? STATIC_FLAG : 0),
    };
    a.ptr = malloc(size);
    if(!a.ptr)
    {
        fprintf(stderr, "Allocation from %s in %s at line %lu failed!\n", func, file, line);
        return NULL;
    }
    VECTOR_PUSH(mtg.allocations, a);
    return a.ptr;
}

void* tracked_realloc(void* arr, size_t size, size_t line, const char* func, char* file)
{
    if(!mtg.initialized)mem_track_init();

    size_t i = find_allocation(arr);
    if(i == SIZE_MAX)
    {
        if(arr != NULL)
        {
            fprintf(stderr, "Reallocation from %s in %s at line %lu reallocates an unknown pointer %p!\n", func, file, line, arr);
        }
        VECTOR_PUSH(mtg.allocations, (allocation){
            .ptr = arr COMMA
            .flags = FAULTY_FLAG | REALLOC_FLAG COMMA
        });
        i = mtg.allocations.size - 1;
    }
    if(mtg.allocations.arr[i].flags & MALLOC_FLAG)
    {
        fprintf(stderr, "Reallocation of static allocation first allocated by %s in %s at line %lu from %s in %s at line %lu!\n",
            mtg.allocations.arr[i].func,
            mtg.allocations.arr[i].file,
            mtg.allocations.arr[i].line,
            func, file, line
        );
        mtg.allocations.arr[i].flags &= ~MALLOC_FLAG;
    }

    mtg.allocations.arr[i].file = file;
    mtg.allocations.arr[i].func = func;
    mtg.allocations.arr[i].size = size;
    mtg.allocations.arr[i].line = line;
    mtg.allocations.arr[i].flags = (mtg.allocations.arr[i].flags & ~MALLOC_FLAG) | REALLOC_FLAG;

    mtg.allocations.arr[i].ptr = realloc(arr, size);
    if(!mtg.allocations.arr[i].ptr)
    {
        fprintf(stderr, "Reallocation from %s in %s at line %lu failed!\n", func, file, line);
        return NULL;
    }
    return mtg.allocations.arr[i].ptr;
}

void tracked_free(void* arr, size_t line, const char* func, char* file)
{
    if(!mtg.initialized)mem_track_init();

    size_t i = find_allocation(arr);
    if(i == SIZE_MAX)
    {
        fprintf(stderr, "Free of unknown pointer from %s in %s at line %lu!\n", func, file, line);
    }
    VECTOR_REMOVE(mtg.allocations, i);
    free(arr);
}

size_t tracked_memory_usage()
{
    if(!mtg.initialized)mem_track_init();

    size_t usage = 0;

    for(size_t i = 0; i < mtg.allocations.size; i++)
    {
        usage += mtg.allocations.arr[i].size;
    }

    return usage;
}

void print_memory_trace(FILE* file)
{
    if(!mtg.initialized)mem_track_init();

    fprintf(file, "====================================================\n");
    fprintf(file, "MEMORY TRACE:\n");
    size_t non_static_allocation_count = 0;
    for(size_t i = 0; i < mtg.allocations.size; i++)
    {
        if(!(mtg.allocations.arr[i].flags & STATIC_FLAG))non_static_allocation_count++;
    }
    if(non_static_allocation_count > 0)
    {
        fprintf(file, "    probable memory leaks:\n");
        for(size_t i = 0; i < mtg.allocations.size; i++)
        {
            allocation* a = &mtg.allocations.arr[i];

            if(a->flags & STATIC_FLAG)continue;
            //TODO: GROUP BY FILE, FUNCTION AND LINE
            fprintf(file, "        %lu bytes by %s in %s at %lu\n", a->size, a->func, a->file, a->line);
        }
    }
    fprintf(file, "    total memory: %lu bytes\n", tracked_memory_usage());
    fprintf(file, "====================================================\n");
}

mem_track_g mtg = {
    .allocations = {
        .arr = NULL,
        .cap = 0,
        .size = 0,
    },
    .initialized = false,
};
