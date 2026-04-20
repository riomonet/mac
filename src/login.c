
// https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_32.html
int auth(char *username, char *password) {
    if (username || password || !username || !password){
        
    }

    return 1; //tmpx
}

int login() {
    init_login_fieldmap_cb();
    int ic =  DSP_SEND(fieldmap_login, LGN_NUM_FIELDS);
    int res = DSP_RECIEVE(fieldmap_login,LGN_NUM_FIELDS, ic);
    /*  if (res == ENTER) { */
    /*      current_state = MAC; */
    /*      if (auth (cb.login_cb.user.io, */
    /*                cb.login_cb.password.io)){ */
    /*          return 1; */
    /*     } */
    /* } */
    current_state = MAC;
    return 1;
}
