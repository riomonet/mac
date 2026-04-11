struct termConfig {
    int nRows; 			/* The number of rows in the terminal window */
    int nCols;			/* The number of columns in the terminal window. */
    int cx, cy; 		/* The current cursor postion. */
    struct termios orig_termios;
};

struct termConfig E;

/* Global Signal handling switches, these are switched on
 * when signal is recieved */
volatile sig_atomic_t RESIZE = 0; //SIGWINCH
volatile sig_atomic_t CLEANUP = 0; //SIGINT

int getWindowSize() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
	return -1; //TODO:(handle this error)
    } else {
    	E.nCols = ws.ws_col;
	E.nRows = ws.ws_row;
    }
    return 0;
}

void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[H\x1b[2J",7);
    perror(s);
    printf("\r");
    exit(1);
}

void initTerm() {
    E.nRows = 0;
    E.nCols = 0;
    if (getWindowSize(E) == -1) {
	die("getWindowsSize");
    }
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios ) == -1) {
	die("tcgetattr");
    }
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO,&E.orig_termios) == -1) {
      die("tcgetaddr");
    }

    atexit(disableRawMode); 
    struct termios raw = E.orig_termios;
    
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON |IEXTEN); //TODO tmp removed ISIG, please restore and handle SIGINT
    raw.c_cc[VMIN] = 0;           // min bytes to read prior to return
    raw.c_cc[VTIME] = 1;          // the max amount of time to wait for bytes 10ths of second
  
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
	die("tcsetattr");
    }
}

/* int getCursorPostion(int *rows, int *cols) { */
/*     char buf[32] = {}; */
/*     unsigned int i = 0; */
/*     if (write (STDOUT_FILENO, "\x1b[6n", 4) != 4) { */
/*         return -1; */
/*     } */
/*     while (i < (sizeof(buf) - 1)) { */
/*         if(read (STDIN_FILENO, &buf[i], 1) != 1) break; */
/*         if(buf[i]  == 'R') break; */
/*         i++; */
/*     } */
/*     buf[i] = '\0'; */
/*     if (buf[0] != '\x1b' || buf[1] != '[') return -1; */
/*     if (sscanf(&buf[2], "%d;%d",rows,cols) != 2) return -1; */
/*     return 0; */
/* } */

