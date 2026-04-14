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

struct ids {
    int client;
    int boat;
    int car;
};

struct client {
    int  cust_id;
    int  status;
    char name_first    [16];
    char name_last     [16];
    char phone         [16];
    char email_address [32];
    char password      [128];
} client;

struct car {
    int  custId;
    char make [32];
    char model [32];
    char color [16];
    char licplate [16];
};

struct cars {
    struct car *cars;
    size_t capacity;
};

struct clients {
    struct client *clients;
    size_t capacity;
};


    

void DSP_CLEANUP(void);
void handler(int);
void setDefaultColors(enum colors background, enum colors foreground);
void term_send_pos(int row, int column);
void term_send_col(enum colors foreground);
void term_send_str(char *str, int len);
void term_send_attr(enum attributes attr);




