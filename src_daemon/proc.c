struct mac_proc proc_table[PROC_TABLE_SIZE] = {0};
  	
int proc_add_to_table(int proc_sockfd) {
    int i = 0;
    for(; i < PROC_TABLE_SIZE; i++) {
	if (proc_table[i].fd != -1) {  // find the next available slot
	    continue;
	}
	proc_table[i].fd = proc_sockfd;
	return i;
    }
    return PROC_ERROR_TABLE;
}

int proc_remove_from_table(int proc_sockfd) {
    for (int i = 0; i <  PROC_TABLE_SIZE; i++) {
	if (proc_table[i].fd == proc_sockfd) {
	    proc_table[i].fd = -1;
	    return 0;
	}
    }
    return PROC_ERROR_NOT_FOUND; // NOT FOUND
}

void proc_initialize_table() {
    for (int i = 0; i < PROC_TABLE_SIZE; i++) {
	proc_table[i].fd = -1;
    }
}

void proc_refresh_table(fd_set *fd_set_ptr) {
  FD_ZERO(fd_set_ptr);
  int i = 0;
  for(; i < PROC_TABLE_SIZE; i++) {
	if (proc_table[i].fd != -1) {
      FD_SET(proc_table[i].fd, fd_set_ptr);
	}
  }
}

	
