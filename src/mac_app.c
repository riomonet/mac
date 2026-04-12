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
#include "helper_macros.h"
#include "colors.h"
#include "attr.h"
#include "term_cmds.h"
#include "term_cmds.c"
#include "mac_app.h"        
#include "term_interop.c"   // RawMode
#include "screens.h"
#include "copybook.c"
#include "DSP.c"            // Display manager



int name_to_idx(struct copybook *cb, char *str) {
    for (int i = 0; i < cb->n_cb_fields; i++) {
        if (strcmp(str, cb->arr[i].name) == 0)
            return i;
    }
    return -1;
}

int auth(char *username, char *password) {
    return 1;
}

int main(void) {
    DSP_start();
    struct copybook *cb_mac = cb_create(fieldmap_mac,
                                        MAX_SLOTS(fieldmap_mac));
    current_state = LOGIN;
    int logged_in = 0;
    while (1) {
        switch (current_state) {
        case LOGIN:
            struct copybook *cb = cb_create(fieldmap_login,
                                            MAX_SLOTS(fieldmap_login));
            while(!logged_in) {
                int ic =  DSP_SEND(fieldmap_login, cb);
                int res = DSP_RECIEVE(fieldmap_login, cb, ic);
                if (res == ENTER) {
                    // replace idx with funccal matching name field
                    if (auth (cb->arr[name_to_idx(cb,"user")].input, 
                              cb->arr[name_to_idx(cb,"password")].input)) {
                        current_state = MAC;
                        memcpy(current_operator,
                               cb->arr[0].input,
                               sizeof(current_operator));
                        cb_free(cb);
                        logged_in = 1;
                    } else {
                        //send failure message
                        // count failures, if failures >3 time out for 10 seconds and                         // reset failure count to 0
                        // reset copybook outputs
                    }
                }
            }
            break;
        case MAC:
            memcpy(cb_mac->arr[name_to_idx(cb_mac,"user")].output,
                   current_operator, 14);
            int ic =  DSP_SEND(fieldmap_mac, cb_mac);
            int res = DSP_RECIEVE(fieldmap_mac, cb_mac, ic);
            break;
        default:
            break;
        }
        if( CLEANUP == 1) {
            DSP_CLEANUP();
            exit(0);
        }
    }
        return 0;
}

