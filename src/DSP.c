/* service provided to mac*/
void term_send_pos(int y, int x) {
    char buf[32];
    snprintf(buf,32,"\x1b[%d;%dH",y,x);
    write(STDOUT_FILENO, buf, strlen(buf));
}

void term_send_col(enum colors col) {
    char *color = colors[col].fg;
    int len =colors[col].fgLen;
    write(STDOUT_FILENO, color, len);
}

void term_send_attr(enum attributes attr) {
    char *a= attribute[attr].seqOn;
    int len =attribute[attr].len;
    write(STDOUT_FILENO, a, len);
}

void term_send_str(char *str, int len) {
    write(STDOUT_FILENO, str,len);
}


void signalHandler(int code) {
    switch (code) {
    case SIGWINCH: RESIZE = 1; break;
    case SIGINT: CLEANUP = 1; break;
    }
}

cb_field *get_cb_field(char *name, cb_field *cb, int num_cb_fields) {
    for(int i = 0; i < num_cb_fields; i++) {
        if (strcmp(name,cb[i].name) == 0)
            return &cb[i];
    }
    return NULL;
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

// TODO: fill out cpy_bk
int DSP_RECIEVE (FIELD *map,cb_field *cb) {
    
    int i = 0;
    int active_field = 0; // index into Map array. Inputs are non sequential.
    int active_idx = 0;   // col number
    int num_inputs = 0;   // total num inputs in cb;
    int active_input = 0; // index into new inputs array;

    /* Find IC and set term to that position */
    while (map[i].row != SENTINEL) {
        if ((map[i].attrb & IC) == IC) {
            active_field = i;
            active_input = num_inputs;
        }
        if ((map[i].attrb & UNPROT) == UNPROT) {
            num_inputs++;
         }
        i++;
    }

    //NOTE: Will need for later to debug Inputs buffer.
    #if 0
    char buf[32];
    term_send_pos(1,1);
    snprintf(buf, 32, "num inputs: %d",num_inputs);
    term_send_str(buf,strlen(buf));
    term_send_pos(2,1);
    snprintf(buf, 32, "ic input %d",active_input);
    term_send_str(buf,strlen(buf));
    #endif
    
    term_send_col(map[active_field].color);
    term_send_attr(UNDERLINE);
    term_send_pos( map[active_field].row,
                   map[active_field].col);
    term_send_cmd(SHOW_CURSOR);
    
    while (1) {
        int c = DSP_read();
        if (c == 'q') {
            DSP_CLEANUP();
        }
        if (isalnum(c)) {
            char ch[1] = {c};
            if (active_idx < map[active_field].len - 1) {
                write(STDOUT_FILENO,ch,1);
                cb[active_input].input[active_idx] = c;
                active_idx++;
                        term_send_pos(1,1);
                        term_send_str(cb[0].input,23);
                        term_send_pos(2,1);
                        term_send_str(cb[1].input,23);
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
                cb[active_input].input[active_idx -1] = ' ';
                active_idx--;
                        term_send_pos(1,1);
                        term_send_str(cb[0].input,23);
                        term_send_pos(2,1);
                        term_send_str(cb[1].input,23);
            } break; 

        case TAB: 
            term_send_cmd(HIDE_CURSOR);
            while(1) {
                active_field++;
                if ((map[active_field].attrb & UNPROT) == UNPROT) {
                    active_idx = 0;
                    active_input++;
                    if (active_input == num_inputs) {
                        active_input = 0;
                    }
                    break; // Break out of inner while loop.
                }
                if (map[active_field].row == SENTINEL) {
                    active_field = 0;
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
            term_send_attr(RESET);
            return 9900;
            
        default: break;
        }
        term_send_pos( map[active_field].row,
                       map[active_field].col + active_idx);
        term_send_cmd(SHOW_CURSOR);
    }
}

// TODO: Draw cbook outputs to the screen at the correct location.
// TODO: Instead of nElments use SENTINEL, the last field in map.
void DSP_SEND(FIELD *map,cb_field *copybook) {
    int i = 0;
    while(map[i].row != SENTINEL) {
        term_send_pos(map[i].row, map[i].col);
        term_send_col(map[i].color);
        if (map[i].attrb & PROT) {
            term_send_str(map[i].initial, map[i].len);
        } else {
            cb_field *f = get_cb_field(map[i].name, copybook, 2);
            term_send_attr(UNDERLINE);
            term_send_str(f->output, 16);

        }
        term_send_cmd(TERM_RESET);
        i++;
    }
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
    enableRawMode();
    initTerm();
    return;
}

void DSP_CLEANUP () {
    term_send_cmd(ORIG_BUFFER);
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(TERM_RESET);
    term_send_cmd(CLEAR_SCREEN);
    term_send_pos(1,1);
    // TODO currently this is Reset to Black, need to query original state.
    printf("\x1b]11;rgb:00/00/00\e\\"); 
    fflush(stdout);
    exit(0);
}

