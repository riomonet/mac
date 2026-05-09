struct mac_socket {
    enum MAC_CONN_PROT {
        MAC_UNIX_T, MAC_INET_T, MAC_HTTP_T, MAC_WEBSOCKET_T
    } prot;

    enum MAC_CONN_CLASS {
        SCK_LISTENER_T, SCK_CLIENT_T, SCK_INIT_T = -1
    } class;
    int fd;
    char *path;
    char *port;
    char *buffer;
};


void net_setup_servers() {
    struct mplx_set mx_set = mplx_create_interface();
    
    struct mac_servers mac_servers = {
	.servers[0] = {
	    .prot = AF_UNIX_T,
	    .path = "/tmp/mac_connect",
	    .port = ""
	}
	.len = 1;
    };
    
    for (int i = 0; i < mac_servers.len; i++){
	int fd = net_start_server(servers[i]);
	mplx_set_add(fd, mx_set);
    }
}


/*
  first add servers to set
  then in a loop add clients to set and
  respond, responses can me callbacks

 */

/*     for(;;) { */
	
/* 	int data_sockfd = accept(master_fd, NULL, NULL); */
/* 	if (data_sockfd == -1) { */
/* 	    perror("accept"); */
/* 	    exit(EXIT_FAILURE); */
/* 	} */
/* 	//DO SOMETHING WITH THE FD */
/*     } else { */
    

/* 	// DO SOMETHING WITH THE FD */
/*     } */
/* } */
/* } */
/* } */
        
/* } */

/*     return 0; */
/* } */
