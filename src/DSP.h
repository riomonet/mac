
#ifndef DSP_H
#define  DSP_H
/* Credit: Salvatore San Fillipo kilo.c, copy and past*/
enum KEY_ACTION {
        KEY_NULL = 0,       /* NULL */
        CTRL_C = 3,         /* Ctrl-c */
        CTRL_D = 4,         /* Ctrl-d */
        CTRL_F = 6,         /* Ctrl-f */
        CTRL_H = 8,         /* Ctrl-h */
        TAB = 9,            /* Tab */
        CTRL_L = 12,        /* Ctrl+l */
        ENTER = 13,         /* Enter */
        CTRL_Q = 17,        /* Ctrl-q */
        CTRL_S = 19,        /* Ctrl-s */
        CTRL_U = 21,        /* Ctrl-u */
        ESC = 27,           /* Escape */
        BACKSPACE =  127,   /* Backspace */
        /* The following are just soft codes, not really reported by the
         * terminal directly. */
        ARROW_LEFT = 1000,
        ARROW_RIGHT,
        ARROW_UP,
        ARROW_DOWN,
        DEL_KEY,
        HOME_KEY,
        END_KEY,
        PAGE_UP,
        PAGE_DOWN,
	F5,F6,F7,F8,F9
	
};

/* Delete:    \x1b[3~ */
/* F5:        \x1b[15~ */
/* F6:        \x1b[17~ */
/* F7:        \x1b[18~ */
/* F8:        \x1b[19~ */
/* F9:        \x1b[20~ */

int DSP_read();
int DSP_RECIEVE (FIELD *map, int num_fields, int ic);
int DSP_SEND(FIELD *map, int num_fields);
void DSP_start();
void DSP_CLEANUP();
#endif
