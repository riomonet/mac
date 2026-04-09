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

typedef struct input {
    char *name;
    char changed;
    char len;
    char attrb;
    char input[128];
    char output[128];
} input;


#define DMS(r, c, l, d, a, clr, n) (FIELD) {     \
 .row      = r,                                 \
 .col      = c,                                 \
 .len      = l,                                 \
 .initial  = d,                                 \
 .color    = clr,                               \
 .attrb    = a,                                 \
 .name     = n,                                 \
 }

#define DCP(n, l) (input) {.name = n, .len = l}
#define SENTINEL -1

struct termConfig E;

void DSP_CLEANUP(void);
void handler(int);
void setDefaultColors(enum colors, enum colors);
void term_send_pos(int, int);
void term_send_col(enum colors col);
void term_send_str(char *str, int len);
void term_send_attr(enum attributes attr);
/* int DSP_read(); */
/* int DSP_send();  */


struct cb {
    int num_inputs;
    input *arr;
};


/* ============================ SCREEN DEFINTIONS ================================*/
FIELD fieldmap_login[] = {                                                      
	DMS( 1, 40, 19, "MARINA 59 | SIGN ON", PROT, WHITE, NULL),
	DMS( 5, 5,  22, "Press Enter to submit:", PROT, MAGENTA, NULL),
	DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, GREEN, NULL),
    DMS( 8, 38, 24, NULL, UNPROT|IC, GREEN, "user"),
	DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT, GREEN, NULL),
    DMS( 10, 38, 24, NULL, UNPROT, GREEN, "password"),
    {.row = SENTINEL}
};



input copybook_login[]= {
    DCP("user", 24),
    DCP("password", 24),
    {.name = NULL}
};


FIELD fieldmap_main[] = {                                                      
	DMS( 1, 40, 21, "MARINA 59 | MAIN MENU", PROT, WHITE, NULL),
    {.row = SENTINEL}
};

input copybook_main[]= {
    DCP("select", 2),
};




/*
  1. choice pass in number of unprots
  2. carete a struct of the array and nInputs; and pass that as the copy book
  3. 

 */
