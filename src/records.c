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
    tbl->rec_id = 1000;
    tbl->rec_index = malloc(start_cap * sizeof(int)); 
    tbl->records = malloc(start_cap * sizeof(struct record));
}

void init_db(struct db *db) {
    init_table(&db->clients, REC_CLIENT);
    init_table(&db->cars, REC_CLIENT);
    init_table(&db->boats, REC_CLIENT);
}

int get_rec_id(struct table *table) {
    if (table->len == table->capacity - 1) {
        size_t new_capacity = 2 * table->capacity;
        table->records = realloc(table->records,new_capacity * sizeof(struct record));
	table->rec_id_lookup = realloc(table->rec_id_lookup, new_capacity * sizeof(int));
        table->capacity = new_capacity;
    }
    table->len++;
    table->rec_id++;
    
    //NOTE: Not an invariant, only for insertion. 
    table->rec_id_lookup[rec_id] = rec_id;
    return rec_id;
}

int db_add_record(struct record rec, enum REC_TYPE type) {
    
            
}


/* load from file */
void db_load() {
}
    





