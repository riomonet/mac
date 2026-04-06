typedef enum {
    PROT = 1,
    UNPROT =2,
    IC = 4
} typ_attr;


typedef struct FIELD {
    int row;
    int col;
    int len;
    char *initial;
    int attrb;
    enum colors color;
    char *name;
} FIELD;

#define FDM(r, c, l, d, a, clr, n) (FIELD) {     \
 .row      = r,                                 \
 .col      = c,                                 \
 .len      = l,                                 \
 .initial  = d,                                 \
 .color    = clr,                               \
 .attrb    = a,                                 \
 .name     = n,                                 \
 }

FIELD x[] = {
    FDM(1,2,3,"ari", PROT, RED,"name")
};

FIELD login[] = {                                                                                                             
	FDM( 1, 30, 19, "MARINA 59 | SIGN ON", PROT, WHITE, NULL),
	FDM( 4, 5,  19, "Press Enter to submit your credentials:", 39, MAGENTA, NULL),
	FDM( 4, 5,  19, "USER . . . . . . . ", PROT, GREEN, NULL),
    FDM( 4, 32, 16, " ", UNPROT|IC, GREEN, "user"),
	FDM( 5, 5,  19, "PASSWORD . . . . . ", PROT, GREEN, NULL),
    FDM( 4, 32, 16, " ", UNPROT, GREEN, "password")
};

void render_Screen(grid *g, FIELD *sc, int nElmnts) {
    for (int i = 0; i < nElmnts; i++) {
        char *fmt = "f";
        if ((sc[i].attrb & UNPROT) == UNPROT) {
            fmt = "fa";
            writeString(g,
                        Pt(sc[i].row,sc[i].col),
                        sc[i].initial,
                        sc[i].len,
                        fmt,
                        sc[i].color,
                        UNDERLINE);
        } else {
            writeString(g,
                        Pt(sc[i].row,sc[i].col),
                        sc[i].initial,
                        sc[i].len,
                        fmt,
                        sc[i].color);
        }
    }
}











