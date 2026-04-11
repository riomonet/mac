FIELD fieldmap_login[] = {                                                      
    DMS( 1, 40, 19, "MARINA 59 | SIGN ON", PROT, _NONE, WHITE, NULL),
    DMS( 5, 5,  22, "Press Enter to submit:", PROT, _NONE, MAGENTA, NULL),
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, _NONE, GREEN, NULL),
    DMS( 8, 38, 16,"",IC, EXT_UNDERLINE, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT,_NONE, GREEN, NULL),
    DMS( 10, 38, 16, "",0, EXT_UNDERLINE, GREEN, "password"),
};

struct copybook *cb_create(FIELD *map, int map_len) {
    int nFields = 0;
    struct copybook *cb = malloc(sizeof(struct copybook));
    cb->cross_map = malloc(map_len * sizeof(cb_field *));
    cb->arr = NULL;
    for(int i = 0; i < map_len; i++) {
        cb->cross_map[i] = NULL;
        if(map[i].name) {
            cb->arr = realloc(cb->arr,
                              (sizeof(cb_field) *
                               (nFields + 1)));
            cb->arr[nFields].name = map[i].name;
            if(!(map[i].attrb & PROT)) {
                cb->arr[nFields].input = malloc(map[i].len);
                cb->arr[nFields].output = malloc(map[i].len);
                memset(cb->arr[nFields].input, ' ',map[i].len);
                memset(cb->arr[nFields].output, ' ',map[i].len);
                cb->cross_map[i] = &cb->arr[nFields];
            }
            nFields++;
        }
    }
    cb->n_cb_fields = nFields;
    cb->n_map_fields = map_len;
    return cb;
}

void cb_login_free(struct copybook * cb) {
    for(int i = 0; i < cb->n_cb_fields; i++) {
        free(cb->arr[i].input);
        free(cb->arr[i].output);
    }
    free(cb->arr);
    free(cb->cross_map);
    free(cb);
}

int auth(char *username, char *password) {
    return 0;
}
