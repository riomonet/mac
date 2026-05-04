#define MAX_SOCKETS 1028

int fdset[MAX_SOCKETS];

void fdset_init() {
    int i;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	fdset[i] = -1;
    }
}

int fdset_add(int fd) {
    int i ;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	if (fdset[i] != -1) continue;
	fdset[i] = fd;
	break;
    }
    return i;
}

int fdset_remove(int fd) {
    int i ;
    for(i = 0; i < MAX_SOCKETS; i++ ) {
	if (fdset[i] == fd) fdset[i] = 1;
	return 0;
    }
    return -1; // NOT FOUND
}

void sys_fdset_refresh(fd_set *fd_set_ptr) {
    FD_ZERO(fd_set_ptr); // fd_set is a bitmap. FD_ZERO zeros the bitmap
    int i;
    for(i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i] != -1) {
	    FD_SET(fdset[i], fd_set_ptr);
	}
    }
}

int fdset_maxfd() {
    int i;
    int max = -1;
    for (i = 0; i < MAX_SOCKETS; i++) {
	if(fdset[i] > max)
	    max  = fdset[i];
    }
    return max;
}
