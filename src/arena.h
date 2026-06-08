#ifndef IVO_ARENA_H
#define IVO_ARENA_H

#include <stdint.h>
#include <stddef.h>
#include "common.h"

typedef size_t arena_offset_t;

typedef struct arena {
    uint8_t* data;
    size_t len;
    size_t cap;
} arena_t;

arena_t create_arena(void);
void destroy_arena(arena_t* arena);
result_t arena_alloc(arena_t* arena, size_t size);
arena_offset_t result_to_offset(result_t result);
void arena_set(arena_t* arena, arena_offset_t offset, const void* src, size_t size);
void* arena_get(arena_t* arena, arena_offset_t offset);

#endif
