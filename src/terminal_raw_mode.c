
int terminal_window_size_get() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
	return -1; //TODO:(handle this error)
    } else {
    	E.nCols = ws.ws_col;
	E.nRows = ws.ws_row;
    }
    return 0;
}

void terminal_die(const char *s) {
    write(STDOUT_FILENO, "\x1b[H\x1b[2J",7);
    perror(s);
    printf("\r");
    exit(1);
}

void terminal_init() {
    E.nRows = 0;
    E.nCols = 0;
    if (terminal_window_size_get(E) == -1) {
       terminal_die("terminal_window_size_get");
    }
}

void raw_mode_disable() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios ) == -1) {
	terminal_die("tcgetattr");
    }
}

void raw_mode_enable() {
    if (tcgetattr(STDIN_FILENO,&E.orig_termios) == -1) {
      terminal_die("tcgetaddr");
    }

    atexit(raw_mode_disable); 
    struct termios raw = E.orig_termios;
    
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON |IEXTEN); //TODO tmp removed ISIG, please restore and handle SIGINT
    raw.c_cc[VMIN] = 0;           // min bytes to read prior to return
    raw.c_cc[VTIME] = 1;          // the max amount of time to wait for bytes 10ths of second
  
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
	terminal_die("tcsetattr");
    }
}


