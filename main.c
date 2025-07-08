#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "helper.h"

void *my_malloc(size_t);
void my_free(void *);

#define HEAP_SIZE 4096
#define MAGIC_NUMBER 12345

node_t *head;
int main(int argc, char*argv[]) {
    // mmap() returns a pointer to a chunk of free space
    head = mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 4096 - sizeof(node_t);
    head->next = NULL;

    // test
    int *p_array = my_malloc(sizeof(int) * 2);
    if (p_array == NULL) {
        fprintf(stderr, "Malloc error\n");
        exit(1);
    }
    for (int i = 0; i < 2; i++) {
        p_array[i] = 0;
        printf("p_array[%d] = %d", i , p_array[i]);
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
            new_block->magic = MAGIC_NUMBER;
            assert(new_block->magic == MAGIC_NUMBER);

            // move head down
            uint8_t *p_temp = (uint8_t *) head + actual_memory_size;
            head = (node_t *) p_temp;
            head->size = HEAP_SIZE - (bytes_requested + sizeof(header_t));
            head->next = NULL;
            p_node = NULL;

            uint8_t *p_temp_new_block = (uint8_t *) new_block + sizeof(header_t);
            void *pointer = (void *) p_temp_new_block;
            return pointer;
        }
        // move pointer along
        p_node = head->next;
    }
    return NULL;
}

void my_free(void *ptr) {
    // free memory
    header_t *p_header = (header_t *) ptr - 1;
    assert(p_header->magic == MAGIC_NUMBER);

    // need to coalesce back into the free list
    node_t *p_node = (node_t *) p_header;
    head->next = (node_t *) p_header;
    p_node->size = p_header->size;
    p_node->next = NULL;
}
