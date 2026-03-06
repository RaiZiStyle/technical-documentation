#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Header {
    size_t count;
    size_t capacity;
} Header;

int* arr_init(size_t init_capacity) {
    // size_t init_capacity = 256;
    Header* header = malloc(sizeof(int) * init_capacity + sizeof(Header));
    header->count = 0;
    header->capacity = init_capacity;
    return (int*)(header + 1);  // same as number = header + sizeof(Header), because ptr + 1 mean ptr + sizeof(ptr)
}

void arr_push(int* arr, int x) {
    Header* header = (Header*)arr - 1;
    printf("header->capacity : %d,", header->capacity);
    printf("header->count : %d\n", header->count);
    assert(header->count < header->capacity);
    arr[header->count++] = x;
}

#define arr_push(arr, x)                                                           \
    do {                                                                           \
        if (arr == NULL) {                                                         \
            Header* header = malloc(sizeof(int) * init_capacity + sizeof(Header)); \
            header->count = 0;                                                     \
            header->capacity = init_capacity;                                      \
            return (int*)(header + 1);       \
            /* same as number = header + sizeof(Header), because ptr + 1 mean ptr + sizeof(ptr) */ \
        }   \
            Header *header = (Header*)(arr) -1;\
            assert(header->count < header->capacity);\
            printf("header->capacity : %d,", header->capacity);\
            printf("header->count : %d\n", header->count);\
            (arr)[header->count++] = (x);\
    } while(0)

// size_t arr_len(int* arr) {
//     return ((Header*)arr - 1)->count;
// }

#define arr_len(arr) ((Header*)(arr) - 1)->count

int main(int argc, char const* argv[]) {
    int* number = arr_init(256);
    arr_push(number, 69);
    arr_push(number, 420);
    arr_push(number, 1337);
    arr_push(number, 20);
    arr_push(number, 6);
    for (size_t i = 0; i < arr_len(number); i++) {
        printf("%d\n", number[i]);
    }

    /* code */
    return 0;
}

///////////////////////////////////////// HASHTABLE with STB
#define STB_DS_IMPLEMENTATION

#include "stb_ds.h"

typedef struct Item {
    const char* key;
    size_t value;
} Item;

Item* table = NULL;

int main2(int argc, char const* argv[]) {
    shput(table, "foo", 69);
    shput(table, "bar", 20);
    shput(table, "baz", 420);

    for (int i = 0; i < shlen(table); i++) {
        printf("%s => %zu\n", table[i].key, table[i].value);
        /* code */
    }

    /* code */
    return 0;
}
