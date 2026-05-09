#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#define T Pool_T
typedef struct T *T;

extern T    Pool_create(void);
extern void Pool_add   (T pool, int elem);
extern void Pool_remove(T pool, int elem);
extern void Pool_free  (T pool);

// Debug Utility
extern void Pool_print (T pool, int cnt);

#undef T
#endif
