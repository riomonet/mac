#ifndef ARRYUTILS_INCLUDED
#define ARRYUTILS_INCLUDED

// Integer Array utils
extern int   iarray_find_first_of (int *x, int len, int tok);
extern void  iarray_print         (int *x, int cnt);
extern int  *iarray_remove_all_of (int *x, int len, int tok);
extern int   iarray_max_value_get (int *x, int len);

#endif
