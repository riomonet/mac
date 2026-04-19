#define REC_ACTIVE 0
#define REC_ARCHIVED 1
#define VERSION_CLIENT 1u << 4

enum REC_TYPE {
    REC_CLIENT = 1u,
    REC_CAR = 1u << 1,
};


#define CLIENT_FNAME_LEN 14
#define CLIENT_LNAME_LEN 14
#define CLIENT_PHONE_LEN 14
#define CLIENT_EMAIL_LEN 16


/* Clients that are not in tables do not have id's */
struct client_record {
    char version_typ;
    char archived;
    u32 id;
    char fname[CLIENT_FNAME_LEN];
    char lname[CLIENT_LNAME_LEN];
    char phone[CLIENT_PHONE_LEN];
    char email[CLIENT_EMAIL_LEN];
};


struct car_record {
    // 4 bits type 4bits version
    char typ_version; 
    char archived;
    u32 id;
    char year[4];
    char state[4];
    char color[8];
    char plate[8];
    char make[12];
    char model[12];
};


struct table_clients {
    int next_avail_id;
    size_t len;
    size_t capacity;
    struct client_record *records;
};

struct table_cars {
    int next_avail_id;
    size_t len;
    size_t capacity;
    struct client_record *records;
};


struct db {
    struct table_clients clients;
};



