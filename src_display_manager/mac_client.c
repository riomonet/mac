
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/un.h>

#include "base_macros.h"
#include "base_typedefs.h"
#include "date_time.h"
#include "terminal_colors.h"
#include "terminal_commands.h"
#include "terminal_raw_mode.h"

#include "bms_constants.h"
#include "fieldmaps.h"
#include "copybook.h"
#include "display_manager.h"

#include "date_time.c"
#include "terminal_raw_mode.c"  
#include "terminal_commands.c"
#include "display_manager.c"            

#define SOCKET_PATH "/tmp/mac_connect"
#define LEN_DATA_BUF 128

int main(void) {

    //    display_manager_start();

    /* Fill out sockaddr_un record. */
    struct sockaddr_un client = {0};
    client.sun_family = AF_UNIX;
    strncpy(client.sun_path, SOCKET_PATH, sizeof(client.sun_path) - 1);

    /* Create a socket.*/
    int client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(client_sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }

    printf("client socket created\n");


    /* connect to server */
    int ret = connect(client_sockfd, (struct sockaddr *)&client, sizeof(client));
    if (ret == -1) {
	perror("connect");
	exit(EXIT_FAILURE);
    }
    printf("connection established\n");

    /* main loop */
    char data_buf[LEN_DATA_BUF];
    while (1) {
	memset(data_buf, 0, LEN_DATA_BUF);
	int nbytes_read = read(STDIN_FILENO , data_buf, LEN_DATA_BUF);
	data_buf[nbytes_read] = 0;
	
	if(strcmp(data_buf, "quit\n") == 0) {
	    break;
	} else {
	    write(client_sockfd, data_buf, nbytes_read + 1);
	}
    }
    
    //    display_manager_cleanup();
}
