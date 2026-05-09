#include <stdio.h>
#include "../listen.h"

int main(void) {
    int fd = start_unix_listener("/tmp/mac_connect");
    printf("fd: %d\n",fd);
    stop_listener(fd);
}
