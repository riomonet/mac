#include<stdio.h>
#include<stdlib.h>
#include "arryutils.h"

int iarray_find_first_of(int *x, int len, int tok) {
    for (int i = 0; i < len; i++) {
        if (x[i] == tok) return i;
    }
    return -1;
}

void iarray_print(int *x, int cnt) {
    for (int i = 0; i< cnt; i++) {
        printf("%d ", x[i]);
    }
}

int *iarray_remove_all_of(int *x, int len, int tok) {
    int cnt = 0, *arr;
    for (int i = 0; i < len; i++) {
        if (x[i] != tok)  cnt++;
    }
    arr = malloc(cnt * sizeof(int));
    int idx = 0;
    for (int i = 0; i < len; i++) {
        if (x[i] != tok) {
            arr[idx++] = x[i];
        }
    }
    return arr;
}

int iarray_max_value_get (int *x, int len) {
    int max = -1;
    for (int i = 0; i < len; i++) {
        if(x[i] > max){
            max  = x[i];
        }
    }
    return max;
}
