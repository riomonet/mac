#include <stdio.h>
#include <sys/select.h>
#include "arryutils.h"
#include "monitor.h"

/* buf size error detection must be as big as capacity */
void Monitor_select(int *elems, int capacity) {
    
    fd_set active_fds;
    FD_ZERO(&active_fds);
    
    for(int i = 0; i < capacity; i++) {
        if(elems[i] != -1) {
            FD_SET(elems[i], &active_fds);
        }
    }
    
    int maxfd = iarray_max_value_get(elems, capacity);


    if ((select((maxfd + 1),&active_fds, NULL, NULL, NULL) == -1)) {
        perror("select");
    }

    int j = 0;
    for(int i = 0; i < capacity; i++) {
        if(FD_ISSET(elems[i], &active_fds)) {
            /* TODO */
        }
    }
}

