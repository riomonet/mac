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


int net_start_server() {
    int server_sockfd;

    struct sockaddr_un server = {0};

    unlink(SOCKET_PATH);

    server_sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(server_sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }
    printf("Master socket created at /tmp/mac_connect\n");

    /* Fill out the 'server_socket' record.*/
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);
    /* Create Master socket */

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
    return server_sockfd;
}



