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

#include "base_macros.h"
#include "base_typedefs.h"
#include "date_time.h"
#include "colors.h"
#include "terminal_commands.h"
#include "terminal_raw_mode.h"  
#include "bms_constants.h"
#include "fieldmaps.h"
#include "copybooks.h"
#include "display_manager.h"
#include "client_records.h"


#include "date_time.c"
#include "terminal_raw_mode.c"  
#include "terminal_commands.c"


#include "fieldmaps.c"
#include "copybook.c"
#include "display_manager.c"            // Display manager

#include "client_records.c"

#include "screen_handler_login.c"
#include "screen_handler_menu_main.c"

int main(void) {
    display_manager_start();
    date_today();
    enum screen_state screen_state = MAC;
    int logged_in = 0;
    while (!logged_in) {
	logged_in = login();
    }

    while (logged_in) {
        switch (screen_state) {

        case MAC:
	    main_menu();
	    break;

	case ADD_NEW_CLIENT:
	    //	    add_new_client();
	    break;
        }
        if( CLEANUP == 1) {
            display_manager_cleanup();
            exit(0);
        }
    }
        return 0;
}

