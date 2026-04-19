
void init_login_fieldmap_cb() {
    FM_INIT(fieldmap_login,
            login_cb,
            LOGIN_FLD_USER,
            user,
            LOGIN_W_USER);
    FM_INIT(fieldmap_login,
            login_cb,
            LOGIN_FLD_PASSWORD,
            password,
            LOGIN_W_PASSWORD);
}


