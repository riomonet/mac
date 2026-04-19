/* A screen is an array of fields */
typedef struct FIELD {
    int row;
    int col;
    int len;
    char *initial;
    char dsp_attr;
    char fld_attr;
    enum colors color; 
    char *name;
} FIELD;


/* Field definitionn macro */
#define DMS(r, c, l, def_val, fld, dsp, clr, n) (FIELD) {  \
 .row        = r,                                 \
 .col        = c,                                 \
 .len        = l,                                 \
 .initial    = def_val,                           \
 .fld_attr  =  fld,      			              \
 .dsp_attr  =  dsp,                               \
 .color      = clr,                               \
 .name       = n,                                 \
 }


FIELD fieldmap_login[] = {                                                      
    DMS( 1, 40, 19, "Marina 59 | Sign On", PROT, NONE, WHITE, NULL),
    DMS( 5, 5,  37, "Tab to change fields, Enter to submit", PROT, NONE, BLUE, NULL),
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, NONE, GREEN, NULL),
    DMS( 8, 38, 16,"",IC, UNDERLINE, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT, NONE, GREEN, NULL),
    DMS( 10, 38, 16, "",0, UNDERLINE, GREEN, "password"),
};


FIELD fieldmap_mac[] = {
    DMS(1,  0, 5, "User:", PROT, NONE, WHITE, NULL),
    DMS(1,  7, 16, "", PROT, NONE,WHITE, "user"),
    DMS(1, 29, 21, "Marina Access Control", PROT, NONE, WHITE, NULL),
    DMS(1, 67, 10, "", PROT,NONE,WHITE,"date"), //TODO: change to named field
    DMS(2, 35,  9, "MAIN MENU", PROT, NONE, WHITE,NULL),
    DMS(2, 67,  8, "",PROT, NONE, WHITE, "time"), //TODO: change to names field
    DMS(6,  6, 28, "Select one of the following:",PROT, NONE, BLUE, NULL),
    DMS(8,10,15,   "1. Add customer",PROT, NONE,GREEN, NULL),
    DMS(9,10,17,   "2. View customers",PROT, NONE,GREEN, NULL),
    DMS(10,10,17,  "3. Access history",PROT, NONE,GREEN, NULL),
    DMS(11,10,15,  "4. Live montior",PROT, NONE,GREEN, NULL),
    DMS(23,0,9,"Selection",PROT,NONE,GREEN,NULL),
    DMS(24,0,4, "===>",PROT,NONE,GREEN,NULL),
    DMS(24,6,96,"",IC,UNDERLINE,GREEN,"select"),
    //Have to give hl a name so it can be rendered from cb.output
    DMS(26,0,100,"",PROT,UNDERLINE,GREEN,"tl"),
    DMS(28,6,9,"F6=Logout",PROT,NONE,BLUE,NULL),  
    DMS(28,19,9, "F7=Search",PROT,NONE,BLUE,NULL),
    DMS(28,31,16,"F8=Redraw screen",PROT,NONE,BLUE,NULL),
    DMS(29,0,100,"",PROT,UNDERLINE,GREEN,"bl"),
};





