#define FM_INIT(fm_name, name_cb, fld_num, fld, width)       \
    fm_name[fld_num].io = (char *)&cb.name_cb.fld.io;        \
    fm_name[fld_num].meta = &cb.name_cb.fld.meta;            \
    memset(cb.name_cb.fld.io,0x20,width);                    \
    cb.name_cb.fld.meta.dsp_attr =                           \
        fm_name[fld_num].dsp_attr;                           \
    cb.name_cb.fld.meta.color =                              \
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

struct login_cb {
    CB_FIELD(user, LGN_LEN_USER);
    CB_FIELD(password, LGN_LEN_PASSWORD);
};

struct client_cb {
    CB_FIELD(fname, 14);
    CB_FIELD(lname, 14);
    CB_FIELD(phone, 14);
    CB_FIELD(email, 16);
};

struct mac_cb {
    CB_FIELD(user, MAC_LEN_USER);
    CB_FIELD(date, MAC_LEN_DATE);
    CB_FIELD(time, MAC_LEN_TIME);
    CB_FIELD(selection, MAC_LEN_SELECTION);
};

struct copybooks {
    struct login_cb login_cb;
    struct mac_cb   mac_cb;
    struct client_cb client_cb;
};

struct copybooks cb;

