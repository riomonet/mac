#ifndef PROC_H
#define PROC_H

#define PROC_TABLE_SIZE 32
#define PROC_ERROR_TABLE -1
#define PROC_ERROR_NOT_FOUND -1
// new client connects, we need to track connections.
// master_file_desciprtor is server_sockfd
// data_sockfd are clients

struct mac_proc {
    int fd;
    char user[16];
    int uid;
    int gid;
    int current_state;
    // if its in the proc table its logged in
    // logged_in_time
    // login attempts
    // timeout
    /* pass the proc  */
};

extern struct mac_proc proc_table[PROC_TABLE_SIZE];



#endif
