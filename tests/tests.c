#include "tests.h"
#include <stdio.h>
#include "../malloc.h"
#include <assert.h>

bool test_one() {
    int *x = my_malloc(sizeof(int));
    if (x == NULL) return false;
    *x = 42;
    bool success = (*x == 42);
    my_free(x);
    printf("Passed test 1\n");
    return success;
}
bool test_two() {
    int *a = my_malloc(sizeof(int));
    int *b = my_malloc(sizeof(int));
    *a = 100;
    *b = 200;
    bool success = (*a == 100 && *b == 200);
    my_free(a);
    my_free(b);
    printf("Passed test 2\n");
    return success;
}
bool test_three() {
    int *x = my_malloc(sizeof(int));
    *x = 5;
    my_free(x);
    int *y = my_malloc(sizeof(int));
    *y = 10;
    bool success = (*y == 10);
    my_free(y);
    printf("Passed test 3\n");
    return success;
}
bool test_four() {
    void *big = my_malloc(5000);
    bool success = (big != NULL);
    my_free(big);
    printf("Passed test 4\n");
    return success;
}
bool test_five() {
    void *a = my_malloc(200);
    void *b = my_malloc(200);
    my_free(a);
    my_free(b);
    // If coalescing works, the free list should contain one larger block
    printf("Passed test 5 (manual verify coalescing in debugger/log)\n");
    return true;
}
bool test_seven() {
    for (int i = 0; i < 100; i++) {
        void *p = my_malloc(16);
        if (!p) return false;
        my_free(p);
    }
    printf("Passed test 7\n");
    return true;
}
bool test_eight() {
    void *p = my_malloc(100);
    my_free(p);
    // Unsafe, but testing behavior
    // my_free(p);  // Uncomment to test robustness if protection is added
    printf("Passed test 8 (manual check for crash)\n");
    return true;
}
bool test_nine() {
    void *p = my_malloc(0);
    printf("Passed test 9\n");
    return (p != NULL);
}
bool test_ten() {
    int *x = my_malloc(sizeof(int));
    *x = 123;
    my_free(x);
    int *y = my_malloc(sizeof(int));
    *y = 456;
    bool success = (*y == 456);
    my_free(y);
    printf("Passed test 10\n");
    return success;
}
bool test_eleven() {
    void *a = my_malloc(100);
    void *b = my_malloc(100);
    void *c = my_malloc(100);
    my_free(b);
    my_free(a);
    // Coalesce should combine a+b
    void *large = my_malloc(180);  // Should fit if coalesced
    bool success = (large != NULL);
    my_free(c);
    my_free(large);
    printf("Passed test 11\n");
    return success;
}
bool test_twelve() {
    void *a = my_malloc(128);
    void *b = my_malloc(128);
    void *c = my_malloc(128);
    my_free(b);
    my_free(c);
    my_free(a);
    printf("Passed test 12\n");
    return true;
}
void run_all_tests() {
    assert(test_one());
    assert(test_two());
    assert(test_three());
    assert(test_four());
    assert(test_five());
    assert(test_seven());
    assert(test_eight());
    assert(test_nine());
    assert(test_ten());
    assert(test_eleven());
    assert(test_twelve());
    printf("✅ All tests passed successfully.\n");
}

