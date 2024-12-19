#include "unordered_list.h"
#include <malloc.h>

#define INITIAL_CAPACITY 8

// Currently these implementations don't account for the possibility that allocations fail.
// You probably have bigger problems if that is the case, though...

void unordered_list_create(UnorderedList* list) {
    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    list->data = malloc(sizeof(void*) * INITIAL_CAPACITY);
}

void unordered_list_push(UnorderedList* list, void* element) {
    if (list->data == NULL) {
        unordered_list_create(list);
    } else if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(void*) * list->capacity);
    }

    list->data[list->size] = element;
    list->size += 1;
}

void unordered_list_fill(UnorderedList* list, int target_index) {
    if (list->data == NULL) {
        return;
    }

    if (target_index >= list->size) {
        return;
    }

    list->size -= 1;
    list->data[target_index] = list->data[list->size];

    if (list->size * 2 < list->capacity) {
        list->capacity = list->size + 1;
        list->data = realloc(list->data, sizeof(void*) * list->capacity);
    }
}

void unordered_list_destroy(UnorderedList* list) {
    list->capacity = 0;
    list->size = 0;
    if (list->data == NULL) {
        free(list->data);
        list->data = NULL;
    }
}