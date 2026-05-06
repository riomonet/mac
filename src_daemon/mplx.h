#ifndef MPLX_H
#define MPLX_H


#define MAX_SOCKETS 1028

struct mplx_set {
    int fd[MAX_SOCKETS];
    int len;
};

struct mplx_set mplx_consumer_create_interface();
void mplx_set_add(int fd, struct mplx_set *mx_set);
void mplx_set_remove(int fd, struct mplx_set *mx_set);
struct mplx_set mplx_wait(struct mplx_set m_set);

#endif
