#ifndef MALLOC_H
#define MALLOC_H

void *my_malloc(size_t);
void my_free(void *);
void coalesce_list();

#endif //MALLOC_H
