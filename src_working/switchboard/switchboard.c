#include "../includes/core.h"

#define BACK_LOG 5
#define PATH "/tmp/mac_c"



int start_unix_listener(char *path) {
    int server_sockfd;
    struct sockaddr_un server = {0};

    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, path, sizeof(server.sun_path) - 1);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
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

int close_socket(int fd) {
    close(fd);
    return 0;
}

/* Using Select syscall to multiplex incoming connections */
void switchboard_select() {
    int unixl_fd = start_unix_listener(PATH);
    Pool_T pool = Pool_create();
    fd_set active_fds;
	
    Pool_add(pool, unixl_fd, UNIX_LISTENER);
    
    for (;;) {
	FD_ZERO(&active_fds);
	for(int i = 0; i < pool->capacity; i++) {
	    if(pool->elems[i].fd != -1) {
		FD_SET(pool->elems[i].fd, &active_fds);
	    }
	}
	int maxfd = -1;
	for (int i = 0; i < pool->capacity; i++) {
	    if(pool->elems[i].fd > maxfd){
		maxfd  = pool->elems[i].fd;
	    }
	}
	
	if ((select((maxfd + 1),&active_fds, NULL, NULL, NULL) == -1)) {
	    perror("select");
	}

	for(int i = 0; i < pool->capacity; i++) {
	    int fd  = pool->elems[i].fd;
	    enum conn_t typ = pool->elems[i].typ;
	    
	    if(FD_ISSET(fd, &active_fds)) {
		if (typ == UNIX_LISTENER) {
		    int client_fd = accept (fd,NULL,NULL);
		    Pool_add(pool, client_fd, UNIX_CLIENT);
		} else if (typ == UNIX_CLIENT) {
		    char buffer[4096];

		    read(fd, buffer, sizeof buffer);
		    int keep_session = client_unix_handler(fd, typ, buffer);
		    write(fd, buffer, strlen(buffer)); 
		    
		    /* if (!keep_session) { */
 		    /* 	/\* 	Pool_remove(pool,fd); NEED TO IMPLEMENT*\/  */
		    /* 	close_socket(fd); */
		    /* } */
		}
	    }
	}
    }
}

