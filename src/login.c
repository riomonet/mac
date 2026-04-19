
// https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_32.html
int auth(char *username, char *password) {
    if (username || password || !username || !password){
        
    }

    return 1; //tmpx
}

int login() {
    init_login_fieldmap_cb();
    int ic =  DSP_SEND(fieldmap_login);
    int res = DSP_RECIEVE(fieldmap_login,len_fieldmap_login, ic);
				
    if (res == ENTER) {
        if (auth (cb.login_cb.user.io,
                  cb.login_cb.password.io)){
            current_state = MAC;
            return 1;
        } else {
            //send failure message
            // count failures, if failures >3 time out for 10 seconds and reset failure count to 0
            // reset copybook outputs
        }
    }
    return 0;
}
