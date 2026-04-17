#include "records.h"

struct record record_create(struct copybook *cb, enum REC_TYPE type) {
    union record_data data;
    char *d = (char *)&data;

    for(int i = 0; i <cb->n_cb_fields; i++) {
        memcpy(d, cb->arr[i].io_buf, cb->arr[i].field_width);
        d += cb->arr[i].field_width;
    }

    struct record rec = {
        .type = type,
        .version = 1,
        .is_active = REC_ACTIVE,
        .data = data,
    };
    return rec;
}

void init_table(struct table *tbl, enum REC_TYPE type) {
    int start_cap = 8;
    tbl->capacity = start_cap;
    tbl->len = 0; //len is the user id.
    tbl->type = type;
    tbl->rows = malloc(start_cap * sizeof(struct record));
}


void init_db(struct db *db) {
    init_table(&db->clients, REC_CLIENT);
    init_table(&db->cars, REC_CLIENT);
    init_table(&db->boats, REC_CLIENT);
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


    






