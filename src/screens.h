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

FIELD fieldmap_mac[] = {                                                      
    DMS(1,  1, 16, "", PROT, NONE,GREEN, "user"),
    DMS(1, 29, 21, "Marina Access Control", PROT, NONE, WHITE, NULL),
    DMS(1, 67, 10, "12/01/2034", PROT,NONE,WHITE,NULL), //TODO: change to named field
    DMS(2, 35,  9, "MAIN MENU", PROT, NONE, WHITE,NULL),
    DMS(2, 67,  8, "10:20 AM",PROT, NONE, WHITE, NULL), //TODO: change to names field
    DMS(6,  6, 44, "Please make a selection and press key Enter:",PROT, NONE, MAGENTA, NULL),
    DMS(8,10,38,   "1. Add a new customer to the database.",PROT, NONE,GREEN, NULL),
    DMS(9,10,38,   "2. View/Edit/Search current customers.",PROT, NONE,GREEN, NULL),
    DMS(10,10,34,  "3. View and search access history.",PROT, NONE,GREEN, NULL),
    DMS(11,10,24,  "4. View live access log.",PROT, NONE,GREEN, NULL),
};

