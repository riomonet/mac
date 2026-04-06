#define POS(r,c)  r, c
#define DFH(r,c,type,str,len, color, name) (field) {.r = r, .c = c, .type = type, }

typedef enum {
    LABEL,
    INPUT,
    DISPLAY
} f_type;

typdef struct {
    int row;
    int col;
    f_type type;
    char text[32];
    int len;
    //attributes
    //other attributs
    colors color;
    char field_name[16];
} field;



struct SCREENS {

    field login[] = {
	DHF( POS(1,30), LABEL, "MARINA 59 | SIGN ON", 19, WHITE, NULL );
	DHF( POS(4,5),  LABEL, "Press Enter to submit your credentials:", 19, MAGENTA, NULL );
	DHF( POS(4,5),  LABEL, "USER.....................", 25, GREEN, NULL );
	DHF( POS(4,9),  INPUT, "", 15, GREEN, "username" );
	DHF( POS(4,5),  LABEL, "PASSWORD", 19, GREEN, NULL );
    }
}
