#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "listen.h"

#define BACK_LOG 5

/* Unchecked Errors
 * 'path' Nul terminated.
 * 'path' not accesable(permissions) */
int start_unix_listener(char *path) {
    int server_sockfd;
    struct sockaddr_un server = {0};

    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, path, sizeof(server.sun_path) - 1);
    server_sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    unlink(path);

    if(server_sockfd == -1) {
        perror("unix domain socket");
        exit(EXIT_FAILURE);
    }
    
    int ret = bind(server_sockfd, (struct sockaddr *)&server, sizeof(server));

    if(ret == -1) {
        perror("unix domain bind failure");
        exit(EXIT_FAILURE);
    }

    ret = listen(server_sockfd, BACK_LOG);
    if(ret == -1) {
        perror("unix domain listen");
        exit(EXIT_FAILURE);
    }
    return server_sockfd;
}

int stop_listener(int fd) {
    close(fd);
}
