int auth(char *username, char *password) {
    if (username || password || !username || !password) {}
    return 1; 
}

int login() {
    bms_init_login();
    int ic =  display_manager_send(fieldmap_login, LGN_NUM_FIELDS);
    int	res = display_manager_recieve(fieldmap_login,LGN_NUM_FIELDS, ic);
    return 1;
}
