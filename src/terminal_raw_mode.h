struct terminal_configuration_record {
    int nRows; 			/* The number of rows in the terminal window */
    int nCols;			/* The number of columns in the terminal window. */
    int cx, cy; 		/* The current cursor postion. */
    struct termios orig_termios;
};

struct terminal_configuration_record E;

/* Global Signal handling switches, these are switched on
 * when signal is recieved */
volatile sig_atomic_t RESIZE = 0; //SIGWINCH
volatile sig_atomic_t CLEANUP = 0; //SIGINT

/*void display_manager_cleanup(void); */
void handler(int);


int terminal_window_size_get();
void terminal_die(const char *s);
void terminal_init();
void raw_mode_disable();
void raw_mode_enable();
