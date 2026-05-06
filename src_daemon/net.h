#ifndef NET_H
#define NET_H

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


/* TODO: Rename this function and make it a generic create header for any tcp ipmessage, not http or wasm
 * move it somewhere else altogether */
u8 *pack_af_unix_msg(field *fm, short fm_len, u8 *cb, short cb_len) {
    assert(fm_len >= 0 && cb_len >= 0);
    assert(fm || cb);

    struct af_unix_header *h;
    u8 *msg = (u8 *) malloc(sizeof(struct af_unix_header) + fm_len + cb_len);
    memcpy(msg + sizeof(struct af_unix_header) + fm_len, cb, cb_len);

    h = (struct af_unix_header *) msg;
    h->version = 1;
    h->cb_len = cb_len;
    h->fm_len = fm_len;
    h->total_len = sizeof(struct af_unix_header) + fm_len + cb_len;
    h->typ = MSG_CB;

    if(fm) {
	memcpy(msg + sizeof(struct af_unix_header), fm, fm_len);
	h->typ = MSG_FMCB;
    } 
    return msg;
}


#endif
