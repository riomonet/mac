#include <stdio.h>
#include "../pool.h"

int main(void) {
    Pool_T p = Pool_create();
    Pool_add(p, 3);
    Pool_add(p, 4);
    Pool_add(p, 5);
    Pool_add(p, 6);
    Pool_add(p, 7);
    Pool_add(p, 8);
    Pool_remove(p,7);
    Pool_remove(p,4);
    Pool_print(p, 10);
    Pool_add(p,7);
    Pool_add(p,4);
    Pool_print(p, 10);
    Pool_free(p);
    Pool_add(p,99);
    //    Pool_print(p, 10);
    return 0;
}



