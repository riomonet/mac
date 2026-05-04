#include "mac_daemon.h"

#include "../common/date_time.c"
#include "fieldmaps.c"
#include "copybook.c"
#include "client_records.c"
#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"
#include "mck_net_mplex.c"
#include "net.c"


int main(void) {

    fd_set system_fd_set;
    fdset_init();
    int master_fd = net_start_server();
    int x = fdset_add(master_fd);
    printf("idx %d\n", x);
    
    bms_init_login();
    bms_init_mac();
    
    /* Main loop for handling connections */

    for(;;) {

	sys_fdset_refresh(&system_fd_set);
	
	select((fdset_maxfd() + 1),&system_fd_set, NULL, NULL, NULL);

	if (FD_ISSET(master_fd,&system_fd_set)) {
	    int data_sockfd = accept(master_fd, NULL, NULL);
	    if (data_sockfd == -1) {
		perror("accept");
		exit(EXIT_FAILURE);
	    }
	    u8 *msg = pack_af_unix_msg(fieldmap_mac,
				       sizeof fieldmap_login,
				       (u8 *)&cb.cb_login,
				       sizeof(cb.cb_login));

	    int af_unix_msg_len = ((struct af_unix_header *) msg)->total_len;
	    write(data_sockfd,msg, af_unix_msg_len);
	    printf("new client: fd %d ,slot: %d\n",data_sockfd,(fdset_add(data_sockfd)));
	    fflush(stdout);
	    
	} else
	    for (int i = 0;i < MAX_SOCKETS; i ++) {
		if (FD_ISSET(fdset[i], &system_fd_set)) {

		    u8 *msg = pack_af_unix_msg(fieldmap_mac,
					       sizeof fieldmap_mac,
					       (u8 *)&cb.cb_mac,
					       sizeof(cb.cb_mac));

		    int af_unix_msg_len = ((struct af_unix_header *) msg)->total_len;
		    write(fdset[i],msg, af_unix_msg_len);
	
		    if (read(fdset[i], &cb.cb_mac,sizeof(cb.cb_mac)) == 0) {
			free(msg);
			break;
		    }
		
		    write(STDOUT_FILENO, cb.cb_login.user.io, 16);
		    write(STDOUT_FILENO, "\n", 1);
		    write(STDOUT_FILENO, cb.cb_login.password.io, 16);
		    free(msg);
		}
	    }
    }
    close(master_fd);
}


