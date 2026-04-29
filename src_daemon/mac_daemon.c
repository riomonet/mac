#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <stddef.h>

#include "../common/base_macros.h"
#include "../common/base_typedefs.h"
#include "../common/date_time.h"
#include "../common/terminal_colors.h"
#include "../common/bms_constants.h"
#include "../common/fieldmaps.h"
#include "../common/copybook.h"
#include "client_records.h"

#include "../common/date_time.c"
#include "fieldmaps.c"
#include "copybook.c"
#include "client_records.c"
#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mac_connect"
#define LEN_DATA_BUF 128
#define BACK_LOG 256


#define MSG_FMCB 1
#define MSG_CB 2


struct af_unix_header {
    u8 version;
    u8 typ;
    u16 fm_len;
    u16 cb_len;
    u16 total_len;
};

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


int main(void) {
#if 1
    int server_sockfd;
    
    struct sockaddr_un server = {0};
    unlink(SOCKET_PATH);

    /* Create Master socket */
    server_sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(server_sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }
    printf("Master socket created at /tmp/mac_connect\n");

    /* Fill out the 'server_socket' record.*/
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);

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
    
    /* Main loop for handling connections */
    for(;;) {

	/* clear the buffer */
	write(STDOUT_FILENO,"Waiting for incoming connections\n", 33);
    
	int data_sockfd = accept(server_sockfd, NULL, NULL);
	if (data_sockfd == -1) {
	    perror("accept");
	    exit(EXIT_FAILURE);
	}
	write(STDOUT_FILENO,"Connection established\n",23);
	bms_init_login();

	u8 *msg = pack_af_unix_msg(fieldmap_login,
				   sizeof fieldmap_login,
				   (u8 *)&cb.cb_login,
				   sizeof(cb.cb_login));

	int af_unix_msg_len = ((struct af_unix_header *) msg)->total_len;
	write(data_sockfd,msg, af_unix_msg_len);
	free(msg);

	//Read response........
	//select epoll etc......
	close(data_sockfd);
    }
    close(server_sockfd);
    #endif
}
