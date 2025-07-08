#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "helper.h"

void *my_malloc(size_t);
void my_free(void *);

#define HEAP_SIZE 4096

// mmap() returns a pointer to a chunk of free space
node_t *head = mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

int main(void) {
    head->size = 4096 - sizeof(node_t);
    head->next = NULL;

    // test
    int *p_array = my_malloc(sizeof(int) * 2);
    if (p_array == NULL) {
        fprintf(stderr, "Malloc error\n");
        exit(1);
    }
    free(p_array);


    return 0;
}

// allocate memory
void *my_malloc(size_t bytes_requested) {
    // actual byte amount
    size_t actual_memory_size = bytes_requested + sizeof(header_t);

    // loop through free list and find first available
    node_t *p_node = head;


    while (p_node != NULL) {
        // first fit algorithm
        if (p_node->size >= actual_memory_size) {
            // create header for new memory block being allocated
            header_t *new_block = (header_t * ) p_node;
            new_block->size = (int) bytes_requested;

            // Will figure out how to do the magic after
            //new_block->magic = ;

            // move head down
            head += actual_memory_size;
            head->size = HEAP_SIZE - (bytes_requested + sizeof(header_t));
            head->next = NULL;
            p_node = NULL;
            return new_block + sizeof(header_t);
        }
        // move pointer along
        p_node = head->next;
    }
    return NULL;
}

void my_free(void *ptr) {
    header_t *p_header = (header_t *) ptr - 1;
    assert(p_header->magic);
}