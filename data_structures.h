#ifndef HELPER_H
#define HELPER_H

// free list structure
typedef struct _node_t {
    int size;
    struct _node_t *next;
} node_t ;

// header when allocating a block
typedef struct {
    int size;
    int magic;
    uintptr_t padding; // 8 byte padding to align header as same size as node for easier work around between them
} header_t;


#endif //HELPER_H
