#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#define ARRAY_SIZE 1024
#define SENTINEL -1

#define T Pool_T
typedef struct T *T;


enum conn_t {
    UNIX_LISTENER, UNIX_CLIENT
};

struct connection {
    int fd;
    enum conn_t typ;
};

struct T {
    struct connection elems[ARRAY_SIZE];
    int capacity;
};

extern T    Pool_create(void);
void Pool_add(T pool, int elem, enum conn_t typ);
extern void Pool_remove(T pool, int elem);
extern void Pool_free  (T pool);
extern int *Pool_get (T pool);
extern int Pool_capacity(T pool);

  
// Debug Utility
extern void Pool_print (T pool, int cnt);

#undef T
#endif
