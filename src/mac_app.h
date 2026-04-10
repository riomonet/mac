/* static enum colors DEF_BG; */
/* static enum colors DEF_FG; */

struct termConfig {
    int nRows; 			/* The number of rows in the terminal window */
    int nCols;			/* The number of columns in the terminal window. */
    int cx, cy; 		/* The current cursor postion. */
    struct termios orig_termios;
};

typedef enum states {
    LOGIN,
    MAC,
    //    ADD_USER,
    //    VIEW_LIVE_LOGS,
    //    SEARCH_LOGS,
    //SENTINEL
} states;

states current_state;

enum t_attrb{
    PROT = 1,
    UNPROT =2,
    IC = 4
};

typedef struct FIELD {
    int row;
    int col;
    int len;
    char *initial;
    int attrb;
    enum colors color;
    char *name;
} FIELD;

typedef struct cb_field {
    char *name;
    short len;
    short attrb;
    char *input;
    char *output;
} cb_field;

#define MAX_SLOTS(arr) sizeof(arr)/sizeof(arr[0])

#define DMS(r, c, l, d, a, clr, n) (FIELD) {     \
 .row      = r,                                 \
 .col      = c,                                 \
 .len      = l,                                 \
 .initial  = d,                                 \
 .color    = clr,                               \
 .attrb    = a,                                 \
 .name     = n,                                 \
 }

#define SENTINEL -1

struct termConfig E;

void DSP_CLEANUP(void);
void handler(int);
void setDefaultColors(enum colors background, enum colors foreground);
void term_send_pos(int row, int column);
void term_send_col(enum colors foreground);
void term_send_str(char *str, int len);
void term_send_attr(enum attributes attr);
/* int DSP_read(); */
/* int DSP_send();  */

char current_operator[16];

/* ============================ SCREEN DEFINTIONS ================================*/

FIELD fieldmap_main[] = {                                                      
	DMS( 1, 40, 21, "MARINA 59 | MAIN MENU", PROT, WHITE, NULL),
    DMS( 4, 5, 40, "Please make a selection and press ENTER:", PROT,MAGENTA,NULL),
    {.row = SENTINEL}
};


