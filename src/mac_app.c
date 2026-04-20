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
#include "bms_constants.h"
#include "colors.h"
#include "attr.h"
#include "term_cmds.h"
#include "term_cmds.c"
#include "mac_app.h"        
#include "term_interop.c"   // RawMode
#include "date_time.c"
#include "screens.h"
#include "copybooks.h"
#include "copybook.c"
#include "DSP.h"
#include "DSP.c"            // Display manager
#include "records.c"
#include "login.c"
#include "main_menu.c"


int main(void) {
    DSP_start();
    date_today();
    current_state = LOGIN;
    int logged_in = 0;

    //TODO: init all copybooks somewhere else. when do i free this one?

    while (1) {
        switch (current_state) {
        case LOGIN: 
            while (!logged_in) {
                logged_in = login();
            } break;
        case MAC: {
            current_state = main_menu();
            logged_in = 1;
        } break;
        case CLIENT: {

            int ic =  DSP_SEND(fieldmap_mac, 5);
            int res = DSP_RECIEVE(fieldmap_mac, 3, ic);

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

