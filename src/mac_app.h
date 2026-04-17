/* static enum colors DEF_BG; */
/* static enum colors DEF_FG; */

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




void DSP_CLEANUP(void);
void handler(int);
void setDefaultColors(enum colors background, enum colors foreground);
void term_send_pos(int row, int column);
void term_send_col(enum colors foreground);
void term_send_str(char *str, int len);
void term_send_attr(enum DSP_ATTR attr);




