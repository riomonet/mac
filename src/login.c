
// https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_32.html
int auth(char *username, char *password) {
    if(username != NULL && password != NULL)
	return 1; //tmp
    return 1; //tmp
}

int login(struct copybook *cb) {
    
    int ic =  DSP_SEND(fieldmap_login, cb);
    int res = DSP_RECIEVE(fieldmap_login, cb, ic);
				
    if (res == ENTER) {
        if (auth (cb->arr[name_to_idx(cb,"user")].io_buf, 
                  cb->arr[name_to_idx(cb,"password")].io_buf)) {
            current_state = MAC;
            memcpy(current_operator,
                   cb->arr[0].io_buf,
                   sizeof(current_operator));
            cb_free(cb);
            return 1;
        } else {
            //send failure message
            // count failures, if failures >3 time out for 10 seconds and reset failure count to 0
            // reset copybook outputs
        }
    }
    return 0;
}
