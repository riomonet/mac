/* TODO: Rename this function and make it a generic create header for any tcp ipmessage, not http or wasm*/
u8 *pack_af_unix_msg(field *fm, short fm_len, u8 *cb, short cb_len) {
    assert(fm_len >= 0 && cb_len >= 0);
    assert(fm || cb);

    struct af_unix_header *h;
    u8 *msg = (u8 *) malloc(sizeof(struct af_unix_header) + fm_len + cb_len);
    memcpy(msg + sizeof(struct af_unix_header) + fm_len, cb, cb_len);

    h = (struct af_unix_header *) msg;
    h->version = 1;
    h->cb_len = cb_len;
    h->fm_len = fm_len;
    h->total_len = sizeof(struct af_unix_header) + fm_len + cb_len;
    h->typ = MSG_CB;

    if(fm) {
	memcpy(msg + sizeof(struct af_unix_header), fm, fm_len);
	h->typ = MSG_FMCB;
    } 
    return msg;
}


enum conn_type {
    AF_UNIX_T, AF_INET_T, HTTP_T, WEBSOCKET_T
};

struct socket_ctx {
    enum conn_type type;
    char *path;
    char *port;
    int (*response)(int fd);
};


int net_start_server(struct socket_ctx ctx) {
    int server_sockfd;
    fd_set system_fd_set; // Multpilex: local copy of the 'select' fdset
    fdset_init();         // 
        
    struct sockaddr_un server = {0};

    switch (ctx.type) {
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
    fdset_add(server_sockfd);

    for(;;) {
	sys_fdset_refresh(&system_fd_set);
	select((fdset_maxfd() + 1),&system_fd_set, NULL, NULL, NULL);
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




