#ifndef MAC_DAEMON_H
#define MAC_DAEMON_H

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
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../common/base_macros.h"
#include "../common/base_typedefs.h"
#include "../common/date_time.h"
#include "../common/terminal_colors.h"
#include "../common/bms_constants.h"
#include "../common/fieldmaps.h"
#include "../common/copybook.h"
#include "client_records.h"
#include "proc.h"
#include "net.h"

#endif
