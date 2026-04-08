#define PT(r, c) {.row = r, .col = c}
#define FMP(rl, cl, re, ce) { .label = { .row = rl, .col = cl },  .entry = { .row = re, .col = ce } }
#define BPT(l,yG,xG) {.len = l, .yGeometry = yG, .xGeometry = xG}
#define NUM_FIELDS(arr) (sizeof(arr) / sizeof((arr)[0]))
#define BASE_PT(g,state) _Pt(g,basePoints[state].len,basePoints[state].yGeometry,basePoints[state].xGeometry)


#define HALF .50
#define ONE_3RD .33
#define TWO_3RD .66
#define ONE_4TH .25
#define THREE_4TH .75
#define ONE_5TH .20
#define TWO_5TH .40
#define THREE_5TH .60
#define FOUR_5TH .80

#define FIELD_LEN 40
#define LABEL_LEN 25

#define USER 0
#define PASSWORD 1

typedef struct point {
    int row;
    int col;
} point;

typedef enum states {
    LOGIN,
    MAC,
    //    ADD_USER,
    //    VIEW_LIVE_LOGS,
    //    SEARCH_LOGS,
    SENTINEL
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
    DCP("password", 24)
};







