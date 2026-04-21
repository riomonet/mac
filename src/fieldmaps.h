typedef struct field {
    int row;
    int col;
    int len;
    char *initial;
    char dsp_attr;
    char fld_attr;
    enum colors color; 
    char *name;  //If not NULL indicates that it is an input or disp  field 
    char *io;
    struct cb_field *meta;
} field;

/* Field definitionn macro */
#define DMS(r, c, l, def_val, fld, dsp, clr, n) (field) {  \
 .row        = r,                                 \
 .col        = c,                                 \
 .len        = l,                                 \
 .initial    = def_val,                           \
 .fld_attr  =  fld,      			              \
 .dsp_attr  =  dsp,                               \
 .color      = clr,                               \
 .name       = n,                                 \
 }
