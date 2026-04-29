/* Field and display attributes used by fieldmaps and copybooks. */
enum DSP_ATTR {
    NONE      =  0,
    UNDERLINE =  1u,
    BLINK     =  1u << 1,
    INVERSE   =  1u << 2,
    STRIKE    =  1U << 3,
    //bits 4 and 5 reserved for future use;
    BOLD      =  0b01000000,
    FAINT     =  0b10000000,
    ITALIC    =  0b11000000
};

enum FLD_ATTR {
    PROT      =  1u,
    IC        =  1u << 1,
    NUMERIC   =  1u << 2,
    HIDDEN    =  1u << 3,
    MDT       =  1u << 4,
    PERSIST   =  1u << 5,
};


enum screen_state {
    MAC,
    ADD_NEW_CLIENT
};




/* LOGIN FIELD CONSTANTS */
#define LGN_IDX_USER 1
#define LGN_IDX_PASSWORD 3

#define LGN_LEN_USER 16
#define LGN_LEN_PASSWORD 16

#define LGN_NUM_FIELDS 6 //len of fieldmap array


/* MAC SCREEN CONSTANTS */
#define MAC_IDX_SELECTION 3

/* #define MAC_LEN_USER 16 */
/* #define MAC_LEN_DATE 10 */
/* #define MAC_LEN_TIME 8 */
#define MAC_LEN_SELECTION 2

#define MAC_NUM_FIELDS 19 // len of fieldmap array

#define CLIENT_LEN_FNAME 16
#define CLIENT_LEN_LNAME 10
#define CLIENT_LEN_PHONE 8
#define CLIENT_LEN_EMAIL 2


