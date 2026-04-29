
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
    int client_sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(client_sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }

    /* connect to server */
    int ret = connect(client_sockfd, (struct sockaddr *)&client, sizeof(client));
    if (ret == -1) {
	perror("connect");
	exit(EXIT_FAILURE);
    }
    
    /* main loop */
    field fieldmap_buf[LEN_DATA_BUF] = {0};
    char cb_buf[4096] = {0};
    int nbytes_fldmap, nbytes_cb, nfields, ic, key;

    while (1) {
	// read type_header -> if fieldmap 
	// read field map
	// read copy book
	// send and recive to/from display manager
	// if just copybook just send cb with cached's fldmap
	// where do i cache it?????? its cached
	/* so if the heade says its a fieldmap then it automaticlly sends a copy book and a field map
	 * and we reset teh fieldmap_buf and reset cb_buf and then do our readsb
	 * if its just a copy book we reset just that and do our reads either
	 * i guess we ar going to rerender either way */
        
	nbytes_fldmap = read(client_sockfd , fieldmap_buf, 4096);
	nbytes_cb = read(client_sockfd , cb_buf, 4096);
	nfields = nbytes_fldmap / sizeof(field);
	ic = display_manager_send(fieldmap_buf, nfields, cb_buf);
	key = display_manager_recieve(fieldmap_buf, nfields, ic, cb_buf);
	// need to send back the key pressed and copybook in a header.
	/* each socket will have its own identity thats how the deamon knows
	   who it is dealing with
	 */
	// write(client_sockfd, cb_buf, nbytes_cb);
    }
    
    display_manager_cleanup();
}
