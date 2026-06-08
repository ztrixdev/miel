#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "arena.h"
#include "common.h"

arena_t create_arena(void) {
    return (arena_t){0};
}

void destroy_arena(arena_t* arena) {
    free(arena->data);
    arena->data = 0;
    arena->len = arena->cap = 0;
}

result_t arena_alloc(arena_t* arena, size_t size) {
    bool realloc_needed = false;
    while (arena->cap < arena->len + size) {
        arena->cap = arena->cap ? (arena->cap * 2) : 256;
        realloc_needed = true;
    }
    if (realloc_needed) {
        uint8_t* new_data = realloc(arena->data, arena->cap);
        if (!new_data) {
            #ifdef ENOMEM
                if (errno == ENOMEM) SIMPLE_ERR("Out of memory");
                else SIMPLE_ERR(strerror(errno));
            #else
                SIMPLE_ERR(strerror(errno));
            #endif
        }
    }
    arena_offset_t* result = malloc(sizeof(arena_offset_t));
    if (!result) {
        #ifdef ENOMEM
            if (errno == ENOMEM) SIMPLE_ERR("Out of memory");
            else SIMPLE_ERR(strerror(errno));
        #else
            SIMPLE_ERR(strerror(errno));
        #endif
    }
    *result = arena->len;
    arena->len += size;
    return (result_t){
        .is_ok = true,
        .payload = { .ok = result }
    };
}

arena_offset_t result_to_offset(result_t result) {
    if (!result.is_ok) return SIZE_MAX;
    if (!result.payload.ok) return SIZE_MAX;
    arena_offset_t offset = *(arena_offset_t*)result.payload.ok;
    free(result.payload.ok);
    result.payload.ok = NULL;
    return offset;
}

void arena_set(arena_t* arena, arena_offset_t offset, const void* src, size_t size) {
    memcpy(arena->data + offset, src, size);
}

void* arena_get(arena_t* arena, arena_offset_t offset) {
    return (void*)(arena->data + offset);
}