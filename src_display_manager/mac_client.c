
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

#define SOCKET_PATH "/tmp/mac_c"
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

    /* connect to server didnt fail?*/
    int ret = connect(client_sockfd, (struct sockaddr *)&client, sizeof(client));
    if (ret < 0) {
	perror("connect");
	exit(1);
    }


#define MSG_FMCB 1
#define MSG_CB 2

    struct af_unix_header {
	u8 version;
	u8 typ;
	u16 fm_len;
	u16 cb_len;
	u16 total_len;
    };

    struct af_unix_data {
	u8 version;
	u16 payload_len;
	int AID;
    };

    u32 ic, key;    
    u32 n_fields = 0, n_bytes = 0;
    u8 af_unix_buf[4096] = {0};
    u8 cb_buf[1024] = {0};
    field fm_array[32];

    while (1) {
	
	n_bytes = read(client_sockfd ,af_unix_buf, sizeof af_unix_buf);
	struct af_unix_header *h = (struct af_unix_header *) af_unix_buf;
	
	assert(h->total_len == n_bytes);
	memcpy(cb_buf, af_unix_buf + sizeof(struct af_unix_header) + h->fm_len, h->cb_len);
	if (h->typ == MSG_FMCB && h->fm_len > 0) {
	    memcpy(fm_array, af_unix_buf + sizeof(struct af_unix_header), h->fm_len);
	}
	n_fields = h->fm_len / sizeof(field);
	ic = display_manager_send(fm_array, n_fields, cb_buf);
	key = display_manager_recieve(fm_array, n_fields, ic, cb_buf);

	struct af_unix_data af_data = {
	    .version = 1,
	    .payload_len = h->cb_len,
	    .AID = key
	};
	u8 *msg = malloc((sizeof af_data) + h->cb_len);
	memcpy(msg + sizeof(af_data),cb_buf, h->cb_len);
	memcpy(msg, &af_data, sizeof(af_data));
	write(client_sockfd, cb_buf, af_data.payload_len + sizeof(af_data));

	for(u32 i = 0; i < n_bytes; i++) {
	    af_unix_buf[i] = 0;
	}
    }
    close(client_sockfd);
    display_manager_cleanup();
}
