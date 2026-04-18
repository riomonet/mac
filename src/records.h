#define REC_INACTIVE 0
#define REC_ACTIVE 1
#define id_offset 1000

enum REC_TYPE {
    REC_CLIENT,
    REC_CAR,
    REC_BOAT
};

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
    char version;           	/* Layout of the struct. */
    enum REC_TYPE type;
    uint32_t record_id;
    char  is_active;        	/* Instead of deleting records become inactive. */
    union record_data data;     /* Array record type. */
    char filler [52];
};

struct table {
    enum REC_TYPE type;
    size_t len;
    size_t capacity;
    uint32_t rec_id;

    /* TODO: THIS DOES NOT MAKE ANY SENSE!!!! */
    /* Lookup table to get the index of 'rec_id' in array 'records'.
     * 'rec_id' start at 1000, if you want to find the index of a record
     * in array 'records', with an id of 1000, dereference index 0 in array
     * 'rec_index'. */
    int rec_id_lookup[1000]; 		
    void *records;
};

struct db {
    struct table clients;
    struct table cars;
    struct table boats;
};

