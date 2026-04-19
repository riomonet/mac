#define FM_INIT(fm_name, name_cb, fld_num, fld, width)       \
    fm_name[fld_num].io = (char *)&cb.name_cb.fld.io;        \
    fm_name[fld_num].meta = &cb.name_cb.fld.meta;            \
    memset(cb.name_cb.fld.io,0x20,width);                    \
    cb.name_cb.fld.meta.dsp_attr =                           \
        fm_name[fld_num].dsp_attr;                           \
    cb.name_cb.fld.meta.color =                              \
        fm_name[fld_num].color;                              \

#define CB_FIELD(name, len)                        \
struct { struct cb_field meta; char io[len];} name\



typedef struct cb_field {
    short len;
    short field_width; 
    char dsp_attr;
    char fld_attr;
    enum colors color;
} cb_field;

enum LOGIN_FLD {
    LOGIN_FLD_USER = 0,
    LOGIN_W_USER = 16,
    LOGIN_FLD_PASSWORD = 1,
    LOGIN_W_PASSWORD = 16,
    LOGIN_N_MAP_FIELDS = 6
};

struct login_cb {
    CB_FIELD(user, 16);
    CB_FIELD(password, 16);
};

struct client_cb {
    CB_FIELD(fname, 14);
    CB_FIELD(lname, 14);
    CB_FIELD(phone, 14);
    CB_FIELD(email, 16);
};

struct mac_cb {
    CB_FIELD(selection, 2);
};

struct copybooks {
    struct login_cb login_cb;
    struct mac_cb   mac_cb;
    struct client_cb client_cb;
};

struct copybooks cb;

