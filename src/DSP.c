

void signalHandler(int code) {
    switch (code) {
    case SIGWINCH: RESIZE = 1; break;
    case SIGINT: CLEANUP = 1; break;
    }
}

int DSP_read() {
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



int DSP_RECIEVE (FIELD *map, struct copybook *cb, int ic) {

    int active_field = ic; // index into Map array. Inputs are non sequential.
    int active_idx = 0;   // col number
    term_send_hlite(cb->cross_map[active_field]->hlite);
    term_send_col(cb->cross_map[active_field]->color);

    while (1) {
        if(RESIZE) {
            DSP_SEND(map, cb);
            term_send_pos(map[active_field].row,
                          map[active_field].col + active_idx);
            term_send_cmd(RESET);
            term_send_hlite(cb->cross_map[active_field]->hlite);
            term_send_col(cb->cross_map[active_field]->color);
            RESIZE = 0;
        }
        int c = DSP_read();
        if (c == 'q') {
            DSP_CLEANUP();
        }
        if (isalnum(c) || c == ' ') {
            char ch[1] = {c};
            if (active_idx < map[active_field].len - 1) {
                write(STDOUT_FILENO,ch,1);
                cb->cross_map[active_field]->input[active_idx] = c;
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
                cb->cross_map[active_field]->input[active_idx - 1] = ' ';
                active_idx--;
            } break; 

        case TAB: 
            term_send_cmd(HIDE_CURSOR);
            term_send_cmd(RESET);
            while(1) {
                active_field++;
                if (active_field == cb->n_map_fields) {
                    active_field = 0 ;
                }
                if (!(map[active_field].attrb & PROT)) {
                    active_idx = 0;
                    break; // Break out of inner while loop.
                }

            }
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
        term_send_hlite(cb->cross_map[active_field]->hlite);
        term_send_col(cb->cross_map[active_field]->color);

    }
}

int DSP_SEND(FIELD *map, struct copybook *cb) {
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(NOWRAP);
    int ic = 0;
    for(int i = 0; i < cb->n_map_fields; i++) {
        if (map[i].attrb & IC) {
            ic = i;
        }
        term_send_pos(map[i].row, map[i].col);
        if (!map[i].name) {
            term_send_col(map[i].color);
            term_send_str(map[i].initial, map[i].len);
        } else {
            term_send_col(cb->cross_map[i]->color);
            term_send_hlite(cb->cross_map[i]->hlite);
            term_send_str(cb->cross_map[i]->output,map[i].len);
        }
        term_send_cmd(RESET);
    }
    term_send_pos(map[ic].row, map[ic].col);
    term_send_cmd(SHOW_CURSOR);
    return ic;
}

void DSP_start() {
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
    term_send_cmd(ALT_BUFFER);
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    printf("\x1b]11;rgb:00/00/00\e\\"); 
    fflush(stdout);
    enableRawMode();
    initTerm();
    return;
}

void DSP_CLEANUP () {
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

