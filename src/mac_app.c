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
            struct copybook *cb = cb_create(fieldmap_login,
                                            MAX_SLOTS(fieldmap_login));
            int logged_in = 0;
            while(!logged_in) {
                int ic =  DSP_SEND(fieldmap_login, cb);
                int res = DSP_RECIEVE(fieldmap_login, cb, ic);
                if (res == ENTER) {
                    if (auth (cb->arr[0].input,
                              cb->arr[1].input)) {
                        current_state = MAC;
                        memcpy(current_operator,
                               cb->arr[0].name,
                               sizeof(current_operator));
                        cb_login_free(cb);
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
