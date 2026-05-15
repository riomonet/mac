#include "../includes/core.h"

#define T Pool_T // it is a pointer. 

T Pool_create() {
    T pool = NULL;
     
    pool = malloc(sizeof *pool);
    if (!pool) {
        perror("Pool_create: malloc failed");
        exit(1);
    }
    
    pool->capacity = ARRAY_SIZE;
    for(int i = 0; i < pool->capacity; i++) {
	pool->elems[i].fd = -1;
    }
    
    return pool;
}

void Pool_add(T pool, int elem, enum conn_t typ) {
    assert(pool);
    int idx = -1;
    
    for (int i = 0; i < pool->capacity; i++) {
        if (pool->elems[i].fd ==  -1){
	    idx = i;
	    break;
	}
    }

    if (idx < 0) {
        perror("Fd pool exceeded capacity");
        exit(1);
    }
    
    pool->elems[idx].fd = elem;
    pool->elems[idx].typ = typ;
}

/* void Pool_remove(T pool, int elem) { */
/*     assert(pool); */

/*     int idx; */
/*     idx = iarray_find_first_of(pool->elems, */
/*                                   pool->capacity, */
/*                                   elem); */
/*     if (idx >= 0) { */
/*         pool->elems[idx] = SENTINEL; */
/*     } */
/* } */


void Pool_free(T pool) {
    if (pool) free(pool);
}


/* void Pool_print (T pool, int cnt) { */
/*     if(cnt > pool->capacity) { */
/*         cnt = pool->capacity; */
/*     } */
/*     iarray_print(pool->elems, cnt); */
/*     printf("\n"); */
/* } */
