#include "mac_daemon.h"

#include "../common/date_time.c"
#include "fieldmaps.c"
#include "copybook.c"
#include "client_records.c"
#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"
#include "proc.c"
#include "net.c"


int main(void) {

    int master_fd = net_start_server();

    bms_init_login();
    bms_init_mac();

    /* wait any accept is getting a login screen ?*/

    
    /* Main loop for handling connections */
    for(;;) {
	
	/* clear the buffer */
	write(STDOUT_FILENO,"Waiting for incoming connections\n", 33);
    
	int data_sockfd = accept(master_fd, NULL, NULL);
	if (data_sockfd == -1) {
	    perror("accept");
	    exit(EXIT_FAILURE);
	}
	
	write(STDOUT_FILENO,"Connection established\n",23);
	
	
	
	for(;;){
	u8 *msg = pack_af_unix_msg(fieldmap_mac,
				   sizeof fieldmap_mac,
				   (u8 *)&cb.cb_mac,
				   sizeof(cb.cb_mac));

	int af_unix_msg_len = ((struct af_unix_header *) msg)->total_len;
	write(data_sockfd,msg, af_unix_msg_len);
	
	if (read(data_sockfd, &cb.cb_mac,sizeof(cb.cb_mac)) == 0) {
	    free(msg);
	    break;
	} 
	write(STDOUT_FILENO, cb.cb_login.user.io, 16);
	write(STDOUT_FILENO, "\n", 1);
	write(STDOUT_FILENO, cb.cb_login.password.io, 16);
	//select epoll etc......
	//	close(data_sockfd);
	free(msg);
	}
	close(data_sockfd);
    }
    close(master_fd);
}


