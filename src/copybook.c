#define DEBUG

typedef struct cb_field {
    // 'name' only used for debug.
    #ifdef DEBUG
    char *name;
    #endif

    // Ownded by DSPM, writted during read. The length
    // of the fiedl up to the last nonspace character.
    // if applicatoin sets len -1, Display manager moves
    // cursor there. 
    short len;

    // Ownded by DSPM. MDT_flag marked when field
    // has been written to by user.
    char field_FA;  

    // Owned by application, can apply
    // brightness,hidden, protected, to change
    // field characteristics during send.
    enum intensity intensity;

    // Owned by application can apply
    // blink, reverse, underline, OFF/normal etc..
    // During send. 
    enum hlite hlite;

    // Owned by applicatoin. Used to change
    // color of field during send.
    enum colors color;
    
    char *input;      //  Owned by Display Manager. 
    char *output;     // Owned by Application. 
} cb_field;


struct copybook {
    int n_map_fields;
    int n_cb_fields;
    cb_field *arr;
    cb_field **cross_map;
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
                cb->arr[nFields].color = map[i].color;
                cb->arr[nFields].hlite = map[i].hlite;
            }
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
        free(cb->arr[i].input);
        free(cb->arr[i].output);
    }
    free(cb->arr);
    free(cb->cross_map);
    free(cb);
}
