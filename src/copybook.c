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
            MAC_IDX_SELECTION,
            selection,
            MAC_LEN_SELECTION);
}




