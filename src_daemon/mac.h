#ifndef MAC_INCLUDED
#define MAC_INCLUDED

int client_unix_handler(int, int, char *buf);

enum scrn_t {
    LOGIN,
    ADD_USER,
};

struct pid_t {
    int session;
    int uid;
    enum scrn_t scrn;
    // some time  stuff for logging
};

struct pid_t pid_table[2048];


#endif
