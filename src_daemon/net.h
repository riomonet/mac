#ifndef NET_H
#define NET_H

#define SOCKET_PATH "/tmp/mac_connect"
#define LEN_DATA_BUF 128
#define BACK_LOG 256

#define MSG_FMCB 1
#define MSG_CB 2

struct af_unix_header {
    u8 version;
    u8 typ;
    u16 fm_len;
    u16 cb_len;
    u16 total_len;
};

#endif
