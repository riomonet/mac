#include "mac_daemon.h"
#include "mac.h"
#include "switchboard/pool.h"

int client_unix_handler(int fd, int typ, char *buf) {
    switch(typ) {
    case UNIX_LISTENER:
	assert(1);
	break;
    case UNIX_CLIENT:break;
	
    }
    
    return 0;
}
