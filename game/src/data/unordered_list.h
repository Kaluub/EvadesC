#pragma once

#include <stdint.h>

typedef struct UnorderedList {
    uint16_t size;
    uint16_t capacity;
    void** data;
} UnorderedList;

void unordered_list_create(UnorderedList* list);
void unordered_list_push(UnorderedList* list, void* element);
void unordered_list_fill(UnorderedList* list, int target_index);
void unordered_list_destroy(UnorderedList* list);