/* A screen is an array of fields */
typedef struct FIELD {
    int row;
    int col;
    int len;
    char *initial;
    enum t_attrb attrb;
    enum hlite hlite; 
    enum colors color;
    char *name;
} FIELD;


/* Field definitionn macro */
#define DMS(r, c, l, d, a, hl, clr, n) (FIELD) {  \
 .row      = r,                                 \
 .col      = c,                                 \
 .len      = l,                                 \
 .initial  = d,                                 \
 .attrb    = a,                                 \
 .hlite    = hl,                                \
 .color    = clr,                               \
 .name     = n,                                 \
 }


FIELD fieldmap_login[] = {                                                      
    DMS( 1, 40, 19, "MARINA 59 | SIGN ON", PROT, NONE, WHITE, NULL),
    DMS( 5, 5,  22, "Press Enter to submit:", PROT, NONE, MAGENTA, NULL),
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, NONE, GREEN, NULL),
    DMS( 8, 38, 16,"",IC, UNDERLINE, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT, NONE, GREEN, NULL),
    DMS( 10, 38, 16, "",0, UNDERLINE, GREEN, "password"),
};

