enum REC_TYPE {
    REC_CLIENT,
    REC_CAR,
    REC_BOAT
};

#define REC_INACTIVE 0
#define REC_ACTIVE 1

#define id_offset 1000

union record_data {
    struct {
        char fname[16];
        char lname[16];
        char phone[16];
        char email[16];
    } client;
    struct {
        char year[4];
        char state[4];
        char color[8];
        char plate[8];
        char make[12];
        char model[12];
    } car;
    struct {
        char len[4];
        char year[4];
        char model[8];
        char make[12];
        char reg[16];
        char name[16]; 
    } boat;
};

struct record {
    enum REC_TYPE type;
    char version;       // feild layout version
    char  status;       // active or inactive
    int id;
    union record_data data;
    char filler [52];
};

struct table {
    enum REC_TYPE type;
    size_t len;
    size_t capacity;
    void *rows;
};

union record_data
record_data_create(struct copybook *cb) {
    union record_data data;
    union record_data *d = &data;

    for(int i = 0; i <cb->n_cb_fields; i++) {
        memcpy(d, cb->arr[i].io_buf, cb->arr[i].field_width);
        d += cb->arr[i].field_width;
    }
    return data;
}

struct record
record_create(union record_data data, enum REC_TYPE type) {
    struct record rec = {
        .type = type,
        .version = 1,
        .status = REC_ACTIVE,
        .data = data,
    };
    return rec;
}


void init_table(struct table *tbl, enum REC_TYPE type) {
    if (tbl->rows == NULL) {
    int start_cap = 8;
    tbl->capacity = start_cap;
    tbl->len = 0; //len is the user id.
    tbl->type = type;
    tbl->rows = malloc(start_cap * sizeof(struct record));
    }
}

struct table clients = {0};

void init_DB() {
    init_table(&clients, REC_CLIENT);
}



//create_table(REC_CLIENT);

/* int get_new_id(struct table *table) { */
/*     if (table->len == table->capacity - 1) { */
/*         int new_capacity = 2 * table->capacity * */
/*             record_size(table->type); */
/*         table->data = realloc(table->data,new_capacity); */
/*         table->capacity = new_capacity; */
/*     } */
/*     table->len++; */
/*     return (table->len + id_offset); */
/* } */


    






