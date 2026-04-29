

void signalHandler(int code) {
    switch (code) {
    case SIGWINCH: RESIZE = 1; break;
    case SIGINT: CLEANUP = 1; break;
    }
}

int display_manager_read() {
    int nread;
    char c, seq[3];
    while ((nread = read(STDIN_FILENO,&c,1)) == 0);
    if (nread == -1) exit(1);

    while(1) {
        switch(c) {
        case ESC:
            /* This is a inadvertent ESC maybe hit once or twice. */
            if (read(STDIN_FILENO,seq,1) == 0) return ESC;
            if (read(STDIN_FILENO,seq+1,1) == 0) return ESC;

            /* This is an actual ESC sequence */
            if (seq[0] == '[') {
                if (seq[1] >= '0' && seq[1] <= '9') {
                    if (read(STDIN_FILENO,seq+2,1) == 0) return ESC;
                    if (seq[2] == '~') {
                        switch(seq[1]) {
                        case '3': return DEL_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        }
                    }
                } else {
                    switch(seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;	      
                    }
                }
            } else if (seq[0] == '0') {
                switch(seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
                }
            }
            break;
        default:
            return c;
        } 
    }
}

int display_manager_recieve (field *map, int map_len, int ic, u8 *base) {
    int active_field = ic; // index into Map array. Inputs are non sequential.
    int active_idx = 0;   // col number

    meta_fields *meta = (meta_fields *)(base + map[active_field].meta_offset);
    u8 *io = (base + map[active_field].io_offset);

    term_send_hlite(meta->dsp_attr);
    term_send_col(meta->color);

    while (1) {
        if(RESIZE) {
            display_manager_send(map, map_len, base);
            term_send_pos(map[active_field].row,
                          map[active_field].col + active_idx);
            term_send_cmd(RESET);
            term_send_hlite(meta->dsp_attr);
            term_send_col(meta->color);
            RESIZE = 0;
        }
        int c = display_manager_read();
        if (c == 'q') {
            display_manager_cleanup();
        }
        if (isalnum(c) || c == ' ') {
            char ch[1] = {c};
            if (active_idx < map[active_field].len - 1) {
                write(STDOUT_FILENO,ch,1);
                io[active_idx] = c;
                active_idx++;
                term_send_pos( map[active_field].row,
                               map[active_field].col + active_idx);
            }
            continue;
        }
        
        switch (c) {
        case BACKSPACE: //Backspace == 127
            if (active_idx > 0) {
                term_send_pos( map[active_field].row,
                               map[active_field].col + active_idx - 1);
                write(STDOUT_FILENO," ",1);
                io[active_idx - 1] = ' ';
                active_idx--;
            } break; 

        case TAB: 
            term_send_cmd(HIDE_CURSOR);
            term_send_cmd(RESET);
            while(1) {
                active_field++;
                if (active_field == map_len) {
                    active_field = 0 ;
                }
                if (!(map[active_field].fld_attr & PROT)) {
                    active_idx = 0;
                    break; // Break out of inner while loop.
                }
	    }
	    meta = (meta_fields *)(base + map[active_field].meta_offset);
	    io = (base + map[active_field].io_offset);
            break;
            
        case ARROW_UP:   break;
        case ARROW_DOWN: break;

        case ARROW_RIGHT:
            if (active_idx < map[active_field].len - 1) {
                active_idx++;                    
            } break;
            
        case ARROW_LEFT:
            if (active_idx > 0) {
                active_idx--;
            } break;
            
        case ENTER:
            term_send_cmd(RESET);
            return ENTER;
            
        default: break;
        }
        term_send_pos(map[active_field].row,
                      map[active_field].col + active_idx);
        term_send_cmd(SHOW_CURSOR);
        term_send_hlite(meta->dsp_attr);
        term_send_col(meta->color);
    }
}

int display_manager_send(field *map, int num_fields, u8 *base) {
    struct date_time dt = date_today();
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(NOWRAP);
    int ic = 0;
    for(int i = 0; i < num_fields; i++) {
        if (map[i].fld_attr & IC) {
            ic = i;
        }

	meta_fields *meta = (meta_fields *)(base + map[i].meta_offset);
	char *io = (char *)(base + map[i].io_offset);

        term_send_pos(map[i].row, map[i].col);
	if (strlen(map[i].name) == 0) {
            term_send_col(map[i].color);
            term_send_hlite(map[i].dsp_attr);
            if(strcmp(map[i].initial,"DSP_TIME") == 0) {
                term_send_str(dt.time, map[i].len); 
            } else if(strcmp(map[i].initial,"DSP_DATE") == 0) {
                term_send_str(dt.date, map[i].len);
            } else if(strcmp(map[i].initial,"DSP_USER") == 0) {
                term_send_str("ariel z", map[i].len);
            }  else if(strcmp(map[i].initial,"DSP_HL") == 0) {
                term_send_str(AUTO_HL, map[i].len);
            } else { 
                term_send_str(map[i].initial, map[i].len);                
            }
	} else {
            term_send_col(meta->color);
            term_send_hlite(meta->dsp_attr);
            term_send_str(io,map[i].len);
	}
        term_send_cmd(RESET);
    }
    term_send_pos(map[ic].row, map[ic].col);
    term_send_cmd(SHOW_CURSOR);
    return ic;
}

void display_manager_start() {
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);    
    sa.sa_flags = SA_RESTART; // Restart interrupted sys-calls.
    sa.sa_handler = signalHandler;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
        perror("sigaction"); 
    }
    // TODO(ari): SIGINT should only be active during debug mode.
    // otherwise SIGINT should be ignored once in raw mode.
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("SIGINT");
    }
    memset(GLOBAL_UNDERLINE,0x20,100);
    term_send_cmd(ALT_BUFFER);
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    printf("\x1b]11;rgb:00/00/00\e\\"); 
    fflush(stdout);
    raw_mode_enable();
    terminal_init();
    return;
}

void display_manager_cleanup () {
    term_send_cmd(ORIG_BUFFER);
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(RESET);
    term_send_cmd(CLEAR_SCREEN);
    term_send_pos(1,1);
    // TODO currently this is Reset to Black, need to query original state.
    printf("\x1b]11;rgb:00/00/00\e\\"); 
    fflush(stdout);
    exit(0);
}

