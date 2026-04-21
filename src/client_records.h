#define CLIENT_RECORD_ACTIVE 0
#define CLIENT_RECORD_ARCHIVED 1
#define CLIENT_RECORD_VERSION 1u << 4


/* Clients that are not in tables do not have id's */
struct client_record {
    char version;
    char archived;
    u32 id;
    char fname[CLIENT_LEN_FNAME];
    char lname[CLIENT_LEN_LNAME];
    char phone[CLIENT_LEN_PHONE];
    char email[CLIENT_LEN_EMAIL];
};

struct client_credentials {
    u32 client_id;
    char password[128];  //embed the salt in the hash
    char n_failed_attempts;
    time_t Locked_until;
};

struct client_record_table {
    int next_avail_id;
    size_t len;
    size_t capacity;
    struct client_record *records;
};

struct client_credentials_table {
    size_t len;
    size_t capacity;
    struct client_credentials *records;
};

    
/* Client record functions. */
struct client_record client_record_create_from_string (char *fname,
						       char *lname,
						       char *phone,
						       char *email);

struct client_record client_record_create (char fname[CLIENT_LEN_FNAME],
					   char lname[CLIENT_LEN_LNAME],
					   char phone[CLIENT_LEN_PHONE],
					   char email[CLIENT_LEN_EMAIL]);

struct client_record_table client_record_table_create();
int client_record_table_get_next_id(struct client_record_table *tbl);
int client_record_table_push (struct client_record_table *tbl, struct client_record *rec);

/* Client credentials api */


 
