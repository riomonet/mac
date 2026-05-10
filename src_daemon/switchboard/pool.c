#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pool.h"
#include "arryutils.h"

#define ARRAY_SIZE 1024
#define SENTINEL -1
#define T Pool_T // it is a pointer. 


struct T {
    int elems[ARRAY_SIZE];
    int capacity;
};

T Pool_create() {
    T pool = NULL;
    
    pool = malloc(sizeof *pool);
    if (!pool) {
        perror("Pool_create: malloc failed");
        exit(1);
    }
    
    pool->capacity = ARRAY_SIZE;
    memset(pool->elems, SENTINEL, pool->capacity);
    
    return pool;
}

void Pool_add(T pool, int elem) {
    assert(pool);
    int idx;
    idx = iarray_find_first_of(pool->elems,
                               pool->capacity,
                               SENTINEL);
    if (idx < 0) {
        perror("Fd pool exceeded capacity");
        exit(1);
    }
    
    pool->elems[idx] = elem;
}

void Pool_remove(T pool, int elem) {
    assert(pool);
    int idx;
    idx = iarray_find_first_of(pool->elems,
                                  pool->capacity,
                                  elem);
    if (idx >= 0) {
        pool->elems[idx] = SENTINEL;
    }
}

void Pool_free(T pool) {
    if (pool) free(pool);
}

void Pool_print (T pool, int cnt) {
    if(cnt > pool->capacity) {
        cnt = pool->capacity;
    }
    iarray_print(pool->elems, cnt);
    printf("\n");
}
