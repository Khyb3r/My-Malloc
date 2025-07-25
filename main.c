#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "data_structures.h"
#include "tests/tests.h"
#include "malloc.h"

#define MAGIC_NUMBER 12345
int HEAP_SIZE = 4096;
node_t *head;
int main(int argc, char*argv[]) {
    // mmap() returns a pointer to a chunk of free space
    head = mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 4096 - sizeof(node_t);
    head->next = NULL;

    // test
    /*
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
    char **words = my_malloc(sizeof(char *) * 3);
    for (int i = 0; i < 3; i++) {
        words[i] = "Hello";
        printf("Word at words[%d]: %s\n", i, words[i]);
    }
    my_free(words);
    */

    // Actual mock unit testing
    //run_all_tests();

    int *x = my_malloc(sizeof(int));
    if (x == NULL) return false;
    *x = 42;
    bool success = (*x == 42);
    my_free(x);
    printf("Passed test 1\n");
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
            int node_size = p_node->size;

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
                new_node->size = (int) (node_size - actual_memory_size);

                // reconfiguring free list
                if (prev_node == NULL) {
                    head = new_node;
                }
                else {
                    prev_node->next = new_node;
                }
                new_node->next = next_node;
            }
            // returning pointer back to user
            uint8_t *p_temp_new_block = (uint8_t *) new_block + sizeof(header_t);
            void *pointer = (void *) p_temp_new_block;
            return pointer;
        }
        prev_node = p_node;
        // move pointer along
        p_node = p_node->next;
    }
    // ran out of memory, increase size of heap
    node_t *new_mem = mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    new_mem->size = 4096 - sizeof(node_t);
    new_mem->next = head;
    head = new_mem;
    return my_malloc(bytes_requested);
}

void my_free(void *ptr) {
    // free memory
    header_t *p_header = (header_t *) ptr - 1;
    assert(p_header->magic == MAGIC_NUMBER);
    int header_size = p_header->size;

    // need to add back into the free list
    node_t *p_node = (node_t *) p_header;
    p_node->size = header_size;
    p_node->next = head;
    head = p_node;

    coalesce_list();
}


void coalesce_list() {
    node_t *curr_node = head;
    node_t *prev_node = NULL;
    while (curr_node != NULL) {
        uint8_t *curr_node_end = (uint8_t *) curr_node + curr_node->size + sizeof(node_t);
        node_t *next_node_start = curr_node->next;
        if (curr_node_end == (uint8_t *)next_node_start) {
            int curr_node_size = curr_node->size;
            int next_node_size = next_node_start->size;
            node_t *p_next_node = curr_node->next->next;
            node_t *new_node = curr_node;
            new_node->size = curr_node_size + next_node_size;
            new_node->next = p_next_node;
            if (prev_node == NULL) {
                head = new_node;
            }
            else {
                prev_node->next = new_node;
            }
            curr_node = new_node;
            continue;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
}
