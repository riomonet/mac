mck_proc_t mck_proc_array[MCK_PROC_ARRAY_SIZE] = {0};

const mck_proc_t mck_proc_default = {
    .fd = PROC_ARRAY_SLOT_AVAILABLE,
    .user = {0},
    .uid = -1,
    .gid = -1,
    .screen_state = -1,
    // if its in the proc table its logged in
    // logged_in_time
    // login attempts
    // timeout
    /* pass the proc  */
};

int mck_proc_array_add(int proc_sockfd) {
    int i;
    for(i = 0; i < MCK_PROC_ARRAY_SIZE; i++) {
	if (PROC_ARRAY_SLOT_OCCUPIED(mck_proc_array[i])) {   // find the next available slot
	    continue;
	}
	PROC_FD(i) = proc_sockfd;
	return i;
    }
    return PROC_ARRAY_ERROR;
}

/* Thid function finds a recod in 'proc_array' by 'proc_state.fd'
 * and resets it to default value, using the 'proc_state_default'
 * template. This makes the record INACTIVE*/
int mck_proc_array_remove(int proc_sockfd) {
    int i;
    for (i = 0; i <  MCK_PROC_ARRAY_SIZE; i++) {
	if (PROC_FD(i) == proc_sockfd) {
	    mck_proc_array[i] = mck_proc_default;
	    return 0;
	}
    }
    return PROC_NOT_FOUND_ERROR; // NOT FOUND
}


/* This function sets every 'proc_state' record in 'proc_array' to
 * a default state, defined in 'proc_state_default' */
void proc_array_initialize() {
    int i;
    for (i = 0; i < MCK_PROC_ARRAY_SIZE; i++) {
	mck_proc_array[i] = mck_proc_default; 
    }
}

/* When using 'select' it is necessary to refresh the kernel fd_set every iteration
 * through the main loop. This function clears the kernel fd_set and then adds 
 * back the 'proc_state.fd' from the 'proc_array' table that
 * are still active */
void proc_array_refresh(fd_set *fd_set_ptr) {
    int i;
    FD_ZERO(fd_set_ptr); // Zero out the select set so there are no fd's
    for(i = 0; i < MCK_PROC_ARRAY_SIZE; i++) {
	if (PROC_ARRAY_SLOT_OCCUPIED(mck_proc_array[i]) ) {
	    FD_SET(PROC_FD(i), fd_set_ptr); // add active proc_array proc_records to the fd_set
	}
    }
}

	
