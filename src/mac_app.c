#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include "lookup_tables.h"
#include "mac_app.h"        
#include "term_interop.c"   // RawMode
#include "DSP.c"            // Display manager
#include "screen_login.c"

int main(void) {
    DSP_start();
    current_state = LOGIN;
    while (1) {
        switch (current_state) {
        case LOGIN:
            cb_field *copybook_login = cb_login_create();
            cb_login_init(copybook_login);
            int logged_in = 0;
            while(!logged_in) {
                DSP_SEND( fieldmap_login,
                          copybook_login,
                          MAX_SLOTS(fieldmap_login));
                int res = DSP_RECIEVE(fieldmap_login, copybook_login);
                if (res == ENTER) {
                    if (auth(copybook_login[0].input,
                             copybook_login[1].input)) {
                        current_state = MAC;
                        memcpy(current_operator,copybook_login[0].name,16);
                        cb_login_free(copybook_login);
                        logged_in = 1;
                    }
                }
            }
            break;
        default:
            break;

            /* if( CLEANUP == 1) { */
            /*     DSP_CLEANUP(); */
            /*     exit(0); */
            /* } */
        }
        return 0;
    }
}
