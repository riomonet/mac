#define AUTO_DSP_USER 16
#define AUTO_DSP_DATE 10
#define AUTO_DSP_TIME 8


typedef struct field {
    int row;
    int col;
    int len;
    char initial[64];
    char dsp_attr;
    char fld_attr;
    enum colors color; 
    char name[32];  //If not NULL indicates that it is an input or disp  field
    size_t io_offset;
    size_t meta_offset;
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




