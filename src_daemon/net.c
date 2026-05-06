#define MAX_SOCKETS 1028

struct mac_socket {
    enum MAC_CONN_PROT {
        AF_UNIX_T, AF_INET_T, HTTP_T, WEBSOCKET_T
    } prot;

    enum MAC_CONN_CLASS {
        SCK_LISTENER_T, SCK_CLIENT_T, SCK_INIT_T = -1
    } class;
    int fd;
    char *path;
    char *port;
    char *buffer;
};

struct mac_socket fdset[MAX_SOCKETS];

void fdset_init() {
    int i;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	fdset[i].class = SCK_INIT_T;
    }
}

int fdset_add(int fd) {
    int i ;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	if (fdset[i].fd != SCK_INIT_T) continue;
	fdset[i].fd = fd;
	break;
    }
    return i;
}

int fdset_remove(int fd) {
    int i ;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
        if (fdset[i].fd == fd) {
            fdset[i].class = SCK_INIT_T;
            fdset[i].fd = SCK_INIT_T;
            return 0;
        }
    }
    return -1; // NOT FOUND
}

void sys_fdset_refresh(fd_set *fd_set_ptr) {
    FD_ZERO(fd_set_ptr); // fd_set is a bitmap. FD_ZERO zeros the bitmap
    int i;
    for(i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i].fd != SCK_INIT_T) {
	    FD_SET(fdset[i].fd, fd_set_ptr);
	}
    }
}

int fdset_maxfd() {
    int i;
    int max = -1;
    for (i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i].fd > max)
	    max  = fdset[i].fd;
    }
    return max;
}


int net_start_server(struct mac_socket ctx) {

    int server_sockfd;


    struct sockaddr_un server = {0};
    switch (ctx.prot) {

    case AF_UNIX_T:
	{
	    server_sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	    server.sun_family = AF_UNIX;
	    strncpy(server.sun_path, ctx.path, sizeof(server.sun_path) - 1);
	    printf("Unix Domain Socket created at /tmp/mac_connect\n");
	    unlink(ctx.path);
	    break;
	}
    case AF_INET_T:
	break;
    case HTTP_T:
	break;
    case WEBSOCKET_T:
	break;
    }

    if(server_sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }

    /* Bind 'server_sockfd' to the socket path named in 'server.sun_path' */
    int ret = bind(server_sockfd, (struct sockaddr *)&server, sizeof(server));
    if(ret == -1) {
	perror("bind");
	exit(EXIT_FAILURE);
    }
    printf("Master socket bind() succesful\n");

    /* Prepare to accept connections.*/
    ret = listen(server_sockfd, BACK_LOG);
    if(ret == -1) {
	perror("listen");
	exit(EXIT_FAILURE);
    }

    /* this is the start of the mplex interfacexs
     * all you shouuld have to do is add an fd
     * and have a wake up function, that gets the
     * fd and thats it, all recroding keeping happens out
     * of the interface. mplex_add(fd, callback) */

    fdset_init();
    fd_set system_fd_set; 
    fdset_add(server_sockfd);

    for(;;) {
	sys_fdset_refresh(&system_fd_set);
	select((fdset_maxfd() + 1),&system_fd_set, NULL, NULL, NULL);
    for (int i = 0; i < MAX_SOCKETS; i++ ) {
        
        if (FD_ISSET(server_sockfd,&system_fd_set)) {
            int data_sockfd = accept(master_fd, NULL, NULL);
            if (data_sockfd == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            //DO SOMETHING WITH THE FD
        } else {
            for (int i = 0;i < MAX_SOCKETS; i ++) {
                if (FD_ISSET(fdset[i], &system_fd_set)) {
                    // DO SOMETHING WITH THE FD
                }
            }
        }
    }
        
    }

    return 0;
}
