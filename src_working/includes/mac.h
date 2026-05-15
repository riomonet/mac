#ifndef MAC_INCLUDED
#define MAC_INCLUDED

int client_unix_handler(int, int, char *buf);

enum scrn_t {
    SCRN_LOGIN,
    SCRN_MAINMNU
};


#endif
