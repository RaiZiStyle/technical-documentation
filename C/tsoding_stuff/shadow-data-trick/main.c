#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION

#include "stb_ds.h"

typedef struct Item {
    const char *key;
    size_t      value;
} Item;

typedef struct Header {
    size_t count;
    size_t capacity;
} Header;

#define ARR_INIT_CAPACITY 1

// int *arr_init(size_t init_capacity) {
//     // size_t init_capacity = 256;
//     Header *header   = malloc(sizeof(int) * init_capacity + sizeof(Header));
//     header->count    = 0;
//     header->capacity = init_capacity;
//     return (int *)(header + 1);  // same as number = header + sizeof(Header), because ptr + 1 mean ptr + sizeof(ptr)
// }

// void arr_push(int* arr, int x) {
//     Header* header = (Header*)arr - 1;
//     printf("header->capacity : %d,", header->capacity);
//     printf("header->count : %d\n", header->count);
//     assert(header->count < header->capacity);
//     arr[header->count++] = x;
// }
#define arr_push(arr, x)                                                                                               \
    do {                                                                                                               \
        if (arr == NULL) {                                                                                             \
            Header *header   = malloc(sizeof(*arr) * ARR_INIT_CAPACITY + sizeof(Header));                              \
            header->count    = 0;                                                                                      \
            header->capacity = ARR_INIT_CAPACITY;                                                                      \
            arr              = (void *)(header + 1);                                                                   \
            /* same as number = header + sizeof(Header), because ptr + 1 mean ptr + sizeof(ptr) */                     \
        }                                                                                                              \
        Header *header = (Header *)(arr) - 1;                                                                          \
        if (header->count >= header->capacity) {                                                                       \
            header->capacity *= 2;                                                                                     \
            header = realloc(header, sizeof(*arr) * header->capacity + sizeof(Header));                                \
            if (header == NULL) {                                                                                      \
                fprintf(stderr, "Failed to reallocate memory\n");                                                      \
                exit(EXIT_FAILURE);                                                                                    \
            }                                                                                                          \
            arr = (void *)(header + 1);                                                                                \
        }                                                                                                              \
        (arr)[header->count++] = (x);                                                                                  \
    } while (0)

// size_t arr_len(int* arr) {
//     return ((Header*)arr - 1)->count;
// }

#define arr_len(arr) ((Header *)(arr) - 1)->count

#define arr_free(arr) free((Header *)(arr) - 1) // we can't free the arr directly because it point to the data, we need to free the header which is before the data

int main(int argc, char const *argv[]) {
    printf("#### Integer array :\n");
    int *numbers = NULL;
    arr_push(numbers, 69);
    arr_push(numbers, 420);
    arr_push(numbers, 1337);
    arr_push(numbers, 20);
    arr_push(numbers, 6);
    if (1) arr_push(numbers, 6969); else arr_push(numbers, 696969);
    for (size_t i = 0; i < arr_len(numbers); i++) {
        printf("%d\n", numbers[i]);
    }

    arr_free(numbers);

    printf("#### Float array :\n");
    float *numberFloat = NULL;
    arr_push(numberFloat, 69);
    arr_push(numberFloat, 420);
    arr_push(numberFloat, 1337);
    arr_push(numberFloat, 20);
    arr_push(numberFloat, 6);
    for (size_t i = 0; i < arr_len(numberFloat); i++) {
        printf("%f\n", numberFloat[i]);
    }

    arr_free(numberFloat);

    printf("#### Hashmap with STB :\n");
    Item *table = NULL;
    shput(table, "foo", 69);
    shput(table, "bar", 20);
    shput(table, "baz", 420);

    for (int i = 0; i < shlen(table); i++) {
        printf("%s => %zu\n", table[i].key, table[i].value);
    }

    shfree(table);

    return 0;
}
