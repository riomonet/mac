#include "records.h"


#if 0
struct client_record new_client_record_cb() {
    
    
}

struct client_record new_client_record_file() {
    struct client_record;
}
#endif

/* String must be NUL terminated. */
struct client_record new_client_record_string(char *fname,
                                              char *lname,
                                              char *phone,
                                              char *email) {
    struct client_record r = {0};
    memset(r.fname, 0x20, sizeof(r.fname));
    memset(r.lname, 0x20, sizeof(r.lname));
    memset(r.phone, 0x20, sizeof(r.phone));
    memset(r.email, 0x20, sizeof(r.email));
    memcpy(r.fname, fname,strlen(fname));
    memcpy(r.lname, lname,strlen(lname));
    memcpy(r.phone, phone,strlen(phone));
    memcpy(r.email, email,strlen(email));
    r.archived = REC_ACTIVE;
    r.version_typ = VERSION_CLIENT | REC_CLIENT;
    return r;
}

/* Fields must be prepadded fixed width. */
struct client_record new_client_record( char fname[CLIENT_FNAME_LEN],
                                        char lname[CLIENT_LNAME_LEN],
                                        char phone[CLIENT_PHONE_LEN],
                                        char email[CLIENT_EMAIL_LEN]) {
    struct client_record r = {0};
    memcpy(r.fname, fname,sizeof(r.fname));
    memcpy(r.lname, lname,sizeof(r.lname));
    memcpy(r.phone, phone,sizeof(r.phone));
    memcpy(r.email, email,sizeof(r.email));
    r.archived = REC_ACTIVE;
    r.version_typ = VERSION_CLIENT | REC_CLIENT;
    return r;
}

struct table_clients init_client_table() {
    struct table_clients tbl = {0};
    int initial_capacity = 8;
    tbl.capacity = initial_capacity;
    tbl.len = 0;
    tbl.next_avail_id = 1000;
    tbl.records = malloc(tbl.capacity * sizeof(struct client_record));
    return tbl;
}

int get_next_client_id(struct table_clients *tbl) {
    return tbl->next_avail_id++;
}

int table_push_client(struct table_clients *tbl, struct client_record *rec) {
    if (tbl->len == tbl->capacity - 1) {
        size_t new_capacity = 2 * tbl->capacity;
        tbl->records = realloc(tbl->records,new_capacity * sizeof(struct client_record));
        tbl->capacity = new_capacity;
    }
    
    rec->id = get_next_client_id(tbl);
    tbl->records[tbl->len] = *rec;
    tbl->len++;

    return rec->id;
}

void init_db(struct db *db) {
    db->clients = init_client_table();
 }



    





