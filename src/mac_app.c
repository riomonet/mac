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

int main(void) {
    DSP_start();
    while (1) {
        DSP_SEND(fieldmap_login,copybook_login,6);
        int EAB = DSP_RECIEVE(fieldmap_login,copybook_login);
        if( CLEANUP == 1) {
            DSP_CLEANUP();
            exit(0);
        }
    }
    return 0;
}
