/* A screen is an array of fields */
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


/* Field definitionn macro */
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


FIELD fieldmap_login[] = {                                                      
    DMS( 1, 40, 19, "MARINA 59 | SIGN ON", PROT, _NONE, WHITE, NULL),
    DMS( 5, 5,  22, "Press Enter to submit:", PROT, _NONE, MAGENTA, NULL),
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, _NONE, GREEN, NULL),
    DMS( 8, 38, 16,"",IC, EXT_UNDERLINE, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT,_NONE, GREEN, NULL),
    DMS( 10, 38, 16, "",0, EXT_UNDERLINE, GREEN, "password"),
};

