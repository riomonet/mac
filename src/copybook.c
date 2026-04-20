void init_login_fieldmap_cb() {
    FM_INIT(fieldmap_login,
            login_cb,
            LGN_IDX_USER,
            user,
            LGN_LEN_USER);
    FM_INIT(fieldmap_login,
            login_cb,
            LGN_IDX_PASSWORD,
            password,
            LGN_LEN_PASSWORD);
}

void init_mac_fieldmap_cb() {
    FM_INIT(fieldmap_mac,
            mac_cb,
            MAC_IDX_USER,
            user,
            MAC_LEN_USER);
    FM_INIT(fieldmap_mac,
            mac_cb,
            MAC_IDX_DATE,
            date,
            MAC_LEN_DATE);
    FM_INIT(fieldmap_mac,
            mac_cb,
            MAC_IDX_TIME,
            time,
            MAC_LEN_TIME);
    FM_INIT(fieldmap_mac,
            mac_cb,
            MAC_IDX_SELECTION,
            selection,
            MAC_LEN_SELECTION);
}




