#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_CAP         640000             // Size of the malloc possible
#define HEAP_ALLOCED_CAP 1024               // Size fo the alloc
#define CHUNK_LIST_CAP   1024               // Number of chunk (of different malloc)

#define UNIMPLEMENTED()                                                                                                \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: %s is not implemented yet\n", __FILE__, __LINE__, __func__);                           \
        abort();                                                                                                       \
    } while (0)


typedef struct Chunk {
    void  *start; // malloc ptr
    size_t size; // size of the malloc
} Chunk;  // struct metadata of what we allocated

typedef struct Chunk_List {
    size_t count; // Number of chunk in the list
    Chunk  chunks[HEAP_ALLOCED_CAP]; // Chunk
} Chunk_List;

char heap[HEAP_CAP] = {0};
// size_t heap_size = 0;

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks   = {.count  = 1,
                             .chunks = {[0] = {.start = heap, .size = sizeof(heap)}}};  // something something comments

int chunk_list_find(const Chunk_List *list, void *ptr) {
    for (size_t i = 0; i < list->count; ++i) {
        if (list->chunks[i].start == ptr) {
            return (int)i;
        }
    }

    return -1;
}
// O(Alloced)
void chunk_list_remove(Chunk_List *list, size_t index) {
    assert(index < list->count);
    for (size_t i = index; i < list->count - 1; ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }
    list->count -= 1;
}

// O(Alloced) ? 
int chunk_list_insert(Chunk_List *list, void *start, size_t size) {
    assert(list->count < CHUNK_LIST_CAP);
    list->chunks[list->count].start = start;
    list->chunks[list->count].size  = size;

    for (size_t i = list->count; i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
        const Chunk t       = list->chunks[i];
        list->chunks[i]     = list->chunks[i - 1];
        list->chunks[i - 1] = t;
    }

    list->count += 1;
    return -1;
}

// Equivalent of malloc
void *heap_alloc(size_t size) {
    //  const size_t size_words = (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    if (size > 0) {
        for (size_t i = 0; i < freed_chunks.count; ++i) {
            const Chunk chunk = freed_chunks.chunks[i];
            if (chunk.size >= size) {
                chunk_list_remove(&freed_chunks, i);

                const size_t tail_size_words = chunk.size - size; // The tails is used to handle case where we have 2 octet of chunk in, but ask to malloc(1), so we push back the 1 octet at the end
                chunk_list_insert(&alloced_chunks, chunk.start, size);

                if (tail_size_words > 0) {
                    chunk_list_insert(&freed_chunks, chunk.start + size, tail_size_words);
                }

                return chunk.start;
            }
        }
    }

    return NULL;
}

// O(Alloced)
void chunk_list_dump(const Chunk_List *list, const char *messageDebug) {
    printf("Chunks %s (%zu):\n", messageDebug, list->count);
    for (size_t i = 0; i < list->count; i++) {
        printf("  start : %p, size : %zu\n", list->chunks[i].start, list->chunks[i].size);
        /* code */
    }
}

// O(Alloced*2)?
void heap_free(void *ptr) {
    if (ptr != NULL) {
        const int index = chunk_list_find(&alloced_chunks, ptr);
        assert(index >= 0);
        assert(ptr == alloced_chunks.chunks[index].start);
        chunk_list_insert(&freed_chunks, alloced_chunks.chunks[index].start, alloced_chunks.chunks[index].size);
        chunk_list_remove(&alloced_chunks, (size_t)index);
    }

    return;
}

void heap_collect() {
    assert(false && "UNIMPLEMENTED: heap_collect is not implemented");
}

int main() {
    printf("Start of the (fake) heap : %p\n", heap);

    for (int i = 0; i < 5; i++) {
        void *p = heap_alloc(i);
        if (i % 2 == 0) {
            heap_free(p);
        }
    }

    printf("####### BEFORE #####\n");
    chunk_list_dump(&alloced_chunks, "alloced");
    chunk_list_dump(&freed_chunks, "freed");

    // heap_alloc(420);
    for (int i = 0; i < 5; i++) {
        heap_alloc(i);
        /* code */
    }
    heap_alloc(3);

    // char *root = heap_alloc(26);
    // for (int i = 0; i < 26; i++){
    //     root[i] = i + 'A';
    // }
    printf("####### AFTER #####\n");
    chunk_list_dump(&alloced_chunks, "alloced");
    chunk_list_dump(&freed_chunks, "freed");

    // heap_dump_alloced_chunks();

    // heap_free(root);
    return 0;
}
