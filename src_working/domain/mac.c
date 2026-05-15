#include "../includes/core.h"

/* #define SCRN_LOGIN 1 */
/* #define SCRN_MAINMNU 2 */

void send_main_menu(int){}
int check_login(char *) {return 0;}
int get_evt(int, char *) {return 0;}

typedef struct session_t {
    int fd;
    int uid;
    enum scrn_t scrn;
} session_t;

session_t *sessions[1024]= {0}; 
    
int client_unix_handler(int fd, int typ, char *buf) {
    switch(typ) {
    case UNIX_LISTENER: // This should never happen.
	assert(1);
	break;
    case UNIX_CLIENT: {
	int is_present = 0;
	for (int i = 0; i < 1024; i++) {
	    if (sessions[i]->fd == fd) {
		is_present = 1;
	    }
	}
	if (!is_present) { // first connection not logged in send login screen
	    struct session_t *new_ses = malloc (sizeof (session_t));
	    new_ses->fd = fd;
	    new_ses->scrn = SCRN_LOGIN;
	    new_ses->uid = -1;
	    
	    for (int i = 0; i < 1024; i++) {
		if (!sessions[i]) {
		    sessions[i] = new_ses;
		    break;
		}
		if (i == 1023) {
		    perror("session array: Filled");
		    exit(1);
		}
	    }
	} else {
	    for (int i = 0; i < 1024;  i++) {
		if(sessions[i]->fd == fd) {
		    struct session_t *ses = sessions[i];
		    switch(ses->scrn) {
		    case SCRN_LOGIN:
			int uid = -1;
			if ((uid = check_login(buf)) > 0){
			    ses->uid = uid;
			    ses->scrn = SCRN_MAINMNU;
			    send_main_menu(ses->fd);
			} break;
		    case SCRN_MAINMNU:
			int key = get_evt(SCRN_MAINMNU, buf);
			break;
		    }
		}
	    }
	}
    } break;
    }
    return 0;
} 
