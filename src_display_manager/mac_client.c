
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

#include "../common/base_macros.h"
#include "../common/base_typedefs.h"
#include "../common/date_time.h"
#include "../common/terminal_colors.h"
#include "terminal_commands.h"
#include "terminal_raw_mode.h"

#include "../common/bms_constants.h"
#include "../common/fieldmaps.h"
#include "../common/copybook.h"
#include "display_manager.h"

#include "../common/date_time.c"
#include "terminal_raw_mode.c"  
#include "terminal_commands.c"
#include "display_manager.c"            

#define SOCKET_PATH "/tmp/mac_connect"
#define LEN_DATA_BUF 256

int main(void) {

    display_manager_start();

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
    field fieldmap_buf[LEN_DATA_BUF];
    char data_buf[4096];
    while (1) {
	memset(fieldmap_buf, 0, LEN_DATA_BUF * sizeof(field));
	int nbytes_read = read(client_sockfd , fieldmap_buf, 4096);
	nbytes_read = read(client_sockfd , data_buf, 4096);
	display_manager_send(fieldmap_buf, 6, data_buf);
	//display_manager_recieve (data_buf, 6, 3);
    }
    display_manager_cleanup();
}
