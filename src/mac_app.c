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
#include "network.c"
#include "helper_macros.h"
#include "colors.h"
#include "attr.h"
#include "term_cmds.h"
#include "term_cmds.c"
#include "mac_app.h"        
#include "term_interop.c"   // RawMode
#include "date_time.c"
#include "screens.h"
#include "copybook.c"
#include "DSP.h"
#include "DSP.c"            // Display manager
#include "records.c"
#include "login.c"
#include "main_menu.c"


int main(void) {
    DSP_start();
    date_today();
    struct db db;
    init_db(&db);
    current_state = LOGIN;
    int logged_in = 0;
    //TODO: init all copybooks somewhere else. when do i free this one?
    struct copybook *cb_mac = cb_create(fieldmap_mac,
                                        MAX_SLOTS(fieldmap_mac));
    while (1) {
        switch (current_state) {
        case LOGIN: {
         
            struct copybook *cb = cb_create(fieldmap_login, MAX_SLOTS(fieldmap_login));
            while (!logged_in) {
                logged_in = login(cb);
            }
        } break;
            
        case MAC: {
            current_state = main_menu(cb_mac);
            logged_in = 0;
            current_state = LOGIN;
        } break;
            
        default: {
        } break;

        }
        if( CLEANUP == 1) {
            DSP_CLEANUP();
            exit(0);
        }
    }
        return 0;
}

