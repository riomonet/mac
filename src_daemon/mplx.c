/* private */
void select_sys_fdset_refresh(fd_set *fd_set_ptr, int fdset[MAX_SOCKETS]) {
    FD_ZERO(fd_set_ptr); // fd_set is a bitmap. FD_ZERO zeros the bitmap
    int i;
    for(i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i] != -1) {
	    FD_SET(fdset[i], fd_set_ptr);
	}
    }
}
/* private */
int mplx_set_maxfd(int fdset[MAX_SOCKETS]) {
    int i;
    int max = -1;
    for (i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i] > max){
	    max  = fdset[i];
	}
    }
    return max;
}


/* private */
void mplx_set_init(struct mplx_set *mx_set) {
    int i;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	mx_set->fd[i] = -1;
    }
}

/* Interace */
struct mplx_set mplx_create_interface() {
    struct mplx_set mx_set = {
	.fd = {0},
	.len = 0
    };
    mplx_set_init(&mx_set);
    return mx_set;
};

/* Interface */
void mplx_set_add(int fd, struct mplx_set *mx_set) {
    int i;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	if (mx_set->fd[i] != -1) continue;
	mx_set->fd[i] = fd;
	mx_set->len++;
	break;
    }
}

/* Interace */
void mplx_set_remove(int fd, struct mplx_set *mx_set) {
    int i ;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
        if (mx_set->fd[i] == fd) {
            mx_set->fd[i] = -1;
	    mx_set->len--;
        }
    }
}

/* interface */
struct mplx_set mplx_wait(struct mplx_set m_set) {
    struct mplx_set nm_set = mplx_create_interface();
    fd_set sys_fd_set;
    select_sys_fdset_refresh(&sys_fd_set, m_set.fd); // Initiate the systems fd list.
    int ret = select((mplx_set_maxfd(m_set.fd) + 1),&sys_fd_set, NULL, NULL, NULL);
    if(ret == -1) {
	(perror("select"));
    }
    for (int i = 0; i < MAX_SOCKETS; i++ ) {
	if (FD_ISSET(m_set.fd[i], &sys_fd_set)) {
	    nm_set.fd[nm_set.len++] = m_set.fd[i];
	}
    }
    return nm_set;
}
