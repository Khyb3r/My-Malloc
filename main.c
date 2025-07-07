#include <stdio.h>
#include <sys/mman.h>
#include "helper.h"

void *malloc(size_t);

int main(void) {
    // mmap() returns a pointer to a chunk of free space
    node_t *head = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 4096 - sizeof(node_t);
    head->next = NULL;
    return 0;
}

// allocate memory
void *malloc(size_t bytes_requested) {
    // actual byte amount
    size_t actual_memory_size = bytes_requested + sizeof(header_t);
    return
}