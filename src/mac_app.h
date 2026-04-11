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
char current_operator[16];

enum t_attrb{
    PROT    = 0b00000001,
    IC      = 0b00000010,
    MDT     = 0b00000100,
    NUMERIC = 0b00001000,
    INTESE  = 0b00010000
};

typedef struct FIELD {
    int row;
    int col;
    int len;
    char *initial;
    enum t_attrb attrb;
    enum ext_attr ext_attr;
    enum colors color;
    char *name;
} FIELD;

typedef struct cb_field {
    // used by SEND() to match cb and fm fields.
    char *name;

    // Ownded by DSPM, writted during read. The length
    // of the fiedl up to the last nonspace character.
    // if applicatoin sets len -1, Display manager moves
    // cursor there. 
    short len;

    // Ownded by DSPM. MDT_flag marked when field
    // has been written to by user.
    char flag;  

    // Owned by application, can apply
    // brightness,hidden, protected, to change
    // field characteristics during send.
    char attrb;

    // Owned by application can apply
    // blink, reverse, underline, OFF/normal etc..
    // During send. 
    char highlight;

    // Owned by applicatoin. Used to change
    // color of field during send.
    enum colors color;
    
    char *input;      //  Owned by Display Manager. 
    char *output;     // Owned by Application. 
} cb_field;

#define MAX_SLOTS(arr) sizeof(arr)/sizeof(arr[0])

#define DMS(r, c, l, d, a, x, clr, n) (FIELD) {  \
 .row      = r,                                 \
 .col      = c,                                 \
 .len      = l,                                 \
 .initial  = d,                                 \
 .color    = clr,                               \
 .attrb    = a,                                 \
 .ext_attr = x,                                 \
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

struct copybook {
    int n_map_fields;
    int n_cb_fields;
    cb_field *arr;
    cb_field **cross_map;
};


/* ============================ SCREEN DEFINTIONS ================================*/

/* FIELD fieldmap_main[] = {                                                       */
/* 	DMS( 1, 40, 21, "MARINA 59 | MAIN MENU", PROT,_NONE, WHITE, NULL), */
/*     DMS( 4, 5, 40, "Please make a selection and press ENTER:", PROT, _NONE, MAGENTA,NULL), */
/*     {.row = SENTINEL} */
/* }; */
