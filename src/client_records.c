#include "records.h"

/* Create a client record from a string.
 *  Strings must be NUL terminated. */
struct client_record
client_record_create_from_string(char *fname,
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
    r.archived = CLIENT_RECORD_ACTIVE;
    r.version = CLIENT_RECORD_VERSION;
    return r;
}

/* Creat a client record form array buffers, buffers must be prepadded
 * with 0x20 fixed width. Used in conjunction with copy books. */
struct client_record
client_record_create ( char fname[CLIENT_LEN_FNAME],
		       char lname[CLIENT_LEN_LNAME],
		       char phone[CLIENT_LEN_PHONE],
		       char email[CLIENT_LEN_EMAIL] ) {
    struct client_record r = {0};
    memcpy(r.fname, fname,sizeof(r.fname));
    memcpy(r.lname, lname,sizeof(r.lname));
    memcpy(r.phone, phone,sizeof(r.phone));
    memcpy(r.email, email,sizeof(r.email));
    r.archived = CLIENT_RECORD_ACTIVE;
    r.version = CLIENT_RECORD_VERSION;
    return r;
}

/* Initialize and return a table that holds client records. */
struct client_record_table
client_record_table_create() {
    struct client_record_table tbl = {0};
    int initial_capacity = 8;
    tbl.capacity = initial_capacity;
    tbl.len = 0;
    tbl.next_avail_id = 1000;
    tbl.records = malloc(tbl.capacity * sizeof(struct client_record));
    return tbl;
}

int client_record_table_get_next_id(struct client_record_table *tbl) {
    return tbl->next_avail_id++;
}

int client_record_table_push(struct client_record_table *tbl, struct client_record *rec) {
    if (tbl->len == tbl->capacity - 1) {
        size_t new_capacity = 2 * tbl->capacity;
        tbl->records = realloc(tbl->records,new_capacity * sizeof(struct client_record));
        tbl->capacity = new_capacity;
    }
    rec->id = client_record_table_get_next_id(tbl);
    tbl->records[tbl->len] = *rec;
    tbl->len++;
    return rec->id;
}



