void bms_init_login() {
    FM_INIT(fieldmap_login,
            cb_login,
            LGN_IDX_USER,
            user,
            LGN_LEN_USER);
    FM_INIT(fieldmap_login,
            cb_login,
            LGN_IDX_PASSWORD,
            password,
            LGN_LEN_PASSWORD);
}

void bms_init_mac() {
    FM_INIT(fieldmap_mac,
            cb_mac,
            MAC_IDX_SELECTION,
            selection,
            MAC_LEN_SELECTION);
}

void bms_init_all() {
    bms_init_login();
    bms_init_mac();
}




