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
//#include "terminal_commands.h"
//#include "terminal_raw_mode.h"

#include "../common/bms_constants.h"
#include "../common/fieldmaps.h"
#include "../common/copybook.h"
//#include "display_manager.h"
#include "client_records.h"

#include "../common/date_time.c"
//#include "terminal_raw_mode.c"  
//#include "terminal_commands.c"

#include "fieldmaps.c"
#include "copybook.c"
//#include "display_manager.c"            // Display manager

#include "client_records.c"

#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mac_connect"
#define LEN_DATA_BUF 128
#define BACK_LOG 256

int main(void) {
#if 1
    int server_sockfd;
    
    struct sockaddr_un server = {0};
    unlink(SOCKET_PATH);

    /* Create Master socket */
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
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

	//send field map login.
	struct msg_head {
	    size_t len;
	    int type;
	};

	write(data_sockfd , fieldmap_login, sizeof fieldmap_login);
	close(data_sockfd);
    }
    close(server_sockfd);
    #endif


}
