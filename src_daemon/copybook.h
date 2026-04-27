#define FM_INIT(fm_name, cb_name, fld_num, fld, width)        \
    fm_name[fld_num].io_offset = offsetof(struct cb_name, fld.io);    \
    fm_name[fld_num].meta_offset = offsetof(struct cb_name, fld.meta);	\
    memset(cb.cb_name.fld.io,0x20,width);                    \
    cb.cb_name.fld.meta.dsp_attr =                           \
        fm_name[fld_num].dsp_attr;                           \
    cb.cb_name.fld.meta.color =                              \
        fm_name[fld_num].color;                              \

#define CB_FIELD(name, len)                       \
    struct { struct meta_fields meta; char io[len];} name

typedef struct meta_fields {
    short len;
    short field_width; 
    char dsp_attr;
    char fld_attr;
    enum colors color;
} meta_fields;

struct cb_login {
    CB_FIELD(user, LGN_LEN_USER);
    CB_FIELD(password, LGN_LEN_PASSWORD);
};

struct cb_mac {
    CB_FIELD(selection, MAC_LEN_SELECTION);
};

struct copybooks {
    struct cb_login cb_login;
    struct cb_mac  cb_mac;
};

global struct copybooks cb;

void bms_init_mac();
void bms_init_login();
void bms_init_all();


/*      struct cb_login { */
/* 	 struct { */
/* 	     struct cb_field meta; */
/* 	     char io[LGN_LEN_USER]; */
/* 	 } user; */
/* 	 struct { */
/* 	     struct cb_field meta; */
/* 	     char io[LGN_LEN_PASSWORD]; */
/* 	 } password; */
/*      } */
