#ifndef PROC_H
#define PROC_H

#define MCK_PROC_ARRAY_SIZE 128  //table of procs
#define PROC_ARRAY_ERROR -1
#define PROC_NOT_FOUND_ERROR -1

#define PROC_ARRAY_SLOT_AVAILABLE -1
#define PROC_ARRAY_SLOT_OCCUPIED(slot) ((slot).fd != -1)
#define PROC_FD(idx) (mck_proc_array[(idx)].fd)
// new client connects, we need to track connections.
// master_file_desciprtor is server_sockfd
// data_sockfd are clients

typedef struct mck_proc_s {
    int fd;
    char user[16];
    int uid;
    int gid;
    int screen_state;
    // if its in the proc table its logged in
    // logged_in_time
    // login attempts
    // timeout
    /* pass the proc  */
} mck_proc_t;


extern mck_proc_t mck_proc_array[MCK_PROC_ARRAY_SIZE];

#endif
