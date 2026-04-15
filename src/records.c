struct client {
    char version;
    char  status;       // active or inactive

    struct {
    char fname[16];
    char lname[16];
    char phone[16];
    char email[16];
    } data;
    char filler [62];
};

struct table {
    TABLE_TYPE type;
    size_t len;
    size_t capacity;
    void *data;
};

enum TABLE_TYPE{
    TABLE_CLIENT,
    TABLE_CAR,
    TABLE_BOAT
};

int next_id(struct table *table) {
 top:
    if (table->len < table->capacity) {
	table->len++;
	return table->len;
    } else {
	switch(table->type) {
	case TABLE_CLIENT:
	    break;
	case TABLE_CAR:
	    break;
	case TABLE_BOAT:
	    break;
	}
    }
}


int add_new_client(struct copybook *cb) {
    /* struct user_data *d = &clients[0].data; */
    /* for (int i = 0; i < cb->n_cb_fields; i++) { */
    /* 	memcpy(d, cb->arr[i].input, cb->arr[i].field_width); */
    /* 	d += cb->arr[i].field_width; */
    /* 	} */
    /* clients[0].rversion = 1; */
    /* clients[0].status = 0; */
}
    






