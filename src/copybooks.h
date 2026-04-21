#define FM_INIT(fm_name, cb_name, fld_num, fld, width)       \
    fm_name[fld_num].io = (char *)&cb.cb_name.fld.io;        \
    fm_name[fld_num].meta = &cb.cb_name.fld.meta;            \
    memset(cb.cb_name.fld.io,0x20,width);                    \
    cb.cb_name.fld.meta.dsp_attr =                           \
        fm_name[fld_num].dsp_attr;                           \
    cb.cb_name.fld.meta.color =                              \
        fm_name[fld_num].color;                              \

#define CB_FIELD(name, len)                       \
struct { struct cb_field meta; char io[len];} name

typedef struct cb_field {
    short len;
    short field_width; 
    char dsp_attr;
    char fld_attr;
    enum colors color;
} cb_field;

struct cb_login {
    CB_FIELD(user, LGN_LEN_USER);
    CB_FIELD(password, LGN_LEN_PASSWORD);
};

struct cb_client {
    CB_FIELD(fname, 14);
    CB_FIELD(lname, 14);
    CB_FIELD(phone, 14);
    CB_FIELD(email, 16);
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

