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
} header_t;


#endif //HELPER_H
