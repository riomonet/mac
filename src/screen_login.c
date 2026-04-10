FIELD fieldmap_login[] = {                                                      
    DMS( 1, 40, 19, "MARINA 59 | SIGN ON", PROT, WHITE, NULL),
    DMS( 5, 5,  22, "Press Enter to submit:", PROT, MAGENTA, NULL),
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, GREEN, NULL),
    DMS( 8, 38, 16, NULL, UNPROT|IC, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT, GREEN, NULL),
    DMS( 10, 38, 16, NULL, UNPROT, GREEN, "password"),
    {.row = SENTINEL}
};
#define MAX_CB_LOGIN_F 2

cb_field *cb_login_create() {
    cb_field *arr = malloc(sizeof(cb_field) *MAX_CB_LOGIN_F);
    arr[0].name = "user";
    arr[1].name = "password";
    return arr;
}

void cb_login_init(cb_field *copybook_login) {
    copybook_login[0].input = malloc(16);
    copybook_login[0].output = malloc(16);
    copybook_login[1].input = malloc(16);
    copybook_login[1].output = malloc(16);
    memset(copybook_login[0].input,' ', 16);
    memset(copybook_login[1].input,' ', 16);
    memset(copybook_login[0].output,' ', 16);
    memset(copybook_login[1].output,' ', 16);
}

void cb_login_free(cb_field *copybook_login) {
    for(int i = 0; i < MAX_CB_LOGIN_F; i++) {
        free(copybook_login[i].input);
        free(copybook_login[i].output);
    }
    free(copybook_login);
}

int auth(char *username, char *password) {
    return 0;
}
