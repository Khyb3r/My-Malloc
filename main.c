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
        printf("p_array[%d] = %d\n", i , p_array[i]);
    }
    my_free(p_array);
    return 0;
}

// allocate memory
void *my_malloc(size_t bytes_requested) {
    // actual byte amount
    size_t actual_memory_size = bytes_requested + sizeof(header_t);

    // loop through free list and find first available node
    node_t *p_node = head;
    node_t *prev_node = NULL;
    while (p_node != NULL) {
        // first fit algorithm
        if (p_node->size >= actual_memory_size) {
            int difference = (int) (p_node->size - actual_memory_size);

            node_t *next_node = p_node->next;

            // create header for new memory block being allocated
            header_t *new_block = (header_t *) p_node;
            new_block->size = (int) bytes_requested;
            new_block->magic = MAGIC_NUMBER;
            assert(new_block->magic == MAGIC_NUMBER);

            if (difference == 0) {
                if (prev_node == NULL) {
                    head = next_node;
                }
                else {
                    prev_node->next = next_node;
                }

            }
            else {
                // create space for allocated block and move node down after the block
                uint8_t *p_temp = (uint8_t *) p_node + actual_memory_size;
                node_t *new_node = (node_t *) p_temp;
                new_node->size = (int) (prev_node->size - actual_memory_size);

                // reconfiguring free list
                if (prev_node == NULL) {
                    head = new_node;
                }
                else {
                    prev_node->next = new_node;
                }
                new_node->next = p_node->next;

                // returning pointer back to user
                uint8_t *p_temp_new_block = (uint8_t *) new_block + sizeof(header_t);
                void *pointer = (void *) p_temp_new_block;
                return pointer;
            }
        }
        prev_node = p_node;
        // move pointer along
        p_node = p_node->next;
    }
    return NULL;
}

void my_free(void *ptr) {
    // free memory
    header_t *p_header = (header_t *) ptr - 1;
    assert(p_header->magic == MAGIC_NUMBER);

    // need to add back into the free list
    node_t *p_node = (node_t *) p_header;
    p_node->size = p_header->size;
    p_node->next = head;
    head = p_node;
}
