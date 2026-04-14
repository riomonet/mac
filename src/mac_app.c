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
#include "screens.h"
#include "copybook.c"
#include "DSP.h"
#include "DSP.c"            // Display manager


struct date_time {
    char date[32];
    char time[32];
};
    
struct date_time date_today() {
    time_t unix_time = time(NULL);
    struct tm *parts = localtime(&unix_time);
    struct date_time dt;
    snprintf(dt.date,32,"%.2d/%.2d/%d",
             parts->tm_mon + 1,
             parts->tm_mday,
             parts->tm_year + 1900);
    snprintf(dt.time,32, "%.2d:%.2d",
             parts->tm_hour,
             parts->tm_min);
    return dt;
}

int auth(char *username, char *password) {
    return 1;
}


int name_to_idx(struct copybook *cb, char *str) {
    for (int i = 0; i < cb->n_cb_fields; i++) {
        if (strcmp(str, cb->arr[i].name) == 0)
            return i;
    }
    return -1;
}

void set_cb_output(struct copybook *cb, char **fields, char **values, int len)  {
    int f_idx;
    for(int i = 0; i < len; i++) {
        f_idx = name_to_idx(cb,fields[i]);
        memcpy(cb->arr[f_idx].output, values[i], strlen(values[i]));
    }
}

/* ============================== PERSIST ============================== */

struct me {
    char name [32];
    char phone [16];
};

struct me create_me(char *name, char *phone) {
    struct me m;
    strcpy(m.name, name);
    strcpy(m.phone, phone);
   return m;
}


struct me spenders() {
    
}



enum asset_type {
    client_t,
    car_t,
    boat_t
};

/*  */
void write_file() {}
void update_file(){}
void read_file() {}
void load_files(){}



/* ============================== MAIN ============================== */
int main(void) {
    DSP_start();
    date_today();
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
            // For screen header display fields
            struct date_time dt = date_today();
            char *fields[] = {"user", "date", "time"};
            char *vals [] =  {current_operator, dt.date, dt.time};
            set_cb_output(cb_mac,fields,vals,3 );
            
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

