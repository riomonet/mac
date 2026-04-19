typedef struct cb_field {
    char *name;
    short len;
    short field_width; 
    char dsp_attr;
    char fld_attr;
    enum colors color;
    char *io_buf;      
} cb_field;

struct copybook {
    int n_map_fields;
    int n_cb_fields;
    cb_field *arr;
    cb_field **cross_map;
};

/* TODO: Maybe, better to count nfields in an initial loop,
 *  then allocate all at once to prevent heap fragmentation */
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
	    cb->arr[nFields].field_width = map[i].len;
	    cb->arr[nFields].io_buf = malloc(map[i].len);
	    memset(cb->arr[nFields].io_buf, ' ',map[i].len);
	    cb->arr[nFields].color = map[i].color;
	    cb->arr[nFields].fld_attr = map[i].fld_attr;
	    cb->arr[nFields].dsp_attr = map[i].dsp_attr;
            nFields++;
        }
    }
    cb->n_cb_fields = nFields;
    cb->n_map_fields = map_len;

    /* Realloc changes the base addrss of 'cb->arr'
     * so we set the values of 'cb->cross_map' int
     * the following loop. */
    int j=0;
    for(int i = 0; i<map_len; i++) {
        if(map[i].name) {
            cb->cross_map[i] = &cb->arr[j++];
        }
    }
    return cb;
}

void cb_free(struct copybook * cb) {
    for(int i = 0; i < cb->n_cb_fields; i++) {
        free(cb->arr[i].io_buf);
    }
    free(cb->arr);
    free(cb->cross_map);
    free(cb);
}

int name_to_idx(struct copybook *cb, char *str) {
    for (int i = 0; i < cb->n_cb_fields; i++) {
        if (strcmp(str, cb->arr[i].name) == 0)
            return i;
    }
    return -1;
}

void set_cb_output(struct copybook *cb, char **fields, char **values, int len)  {
    int f_idx;
    for(int i = 0; i < len; i++) {
        f_idx = name_to_idx(cb,fields[i]);
        memcpy(cb->arr[f_idx].io_buf, values[i], strlen(values[i]));
    }
}
