#include "mac_daemon.h"

#include "../common/date_time.c"
#include "fieldmaps.c"
#include "copybook.c"
#include "client_records.c"
#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"
#include "mplx.c"
#include "net.c"


int main(void) {
    net_setup_servers();
    net_event_loop_start();
  
    #if 0
    u8 *msg = pack_af_unix_msg(fieldmap_mac,
			       sizeof fieldmap_login,
			       (u8 *)&cb.cb_login,
			       sizeof(cb.cb_login));

    // reads and writes. 
    int af_unix_msg_len = ((struct af_unix_header *) msg)->total_len;
    write(data_sockfd,msg, af_unix_msg_len);
    printf("new client: fd %d ,slot: %d\n",data_sockfd,(fdset_add(data_sockfd)));
    fflush(stdout);
	    
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
	close(master_fd);
    #endif
}



