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

void DSP_CLEANUP(void);
int DSP_RECIEVE (FIELD *map,input *cbook) {
    // find home and place the cursor
    int i = 0;
    int active_field = 0;
    int active_idx = 0;
    term_send_col(GREEN);
    term_send_attr(UNDERLINE);

    /* Find IC and set term to that position */
    while (map[i].row != SENTINEL) {
        if((map[i].attrb & IC) == IC) {
            active_field = i;
            break;
        } i++;
    }

    term_send_pos( map[active_field].row,
                   map[active_field].col);
    term_send_cmd(SHOW_CURSOR);

    while (1) {
        int c = DSP_read();
        if (c == 'q') {
            DSP_CLEANUP();
            exit(0);
        }

        if (isalnum(c)) {
            char ch[1] = {c};
            if (active_idx < map[i].len) {
                write(STDOUT_FILENO,ch,1);
                active_idx++;
                term_send_pos( map[active_field].row,
                               map[active_field].col + active_idx);
            }
            continue;
        } switch (c) {

        case (127): //Backspace == 127
            if (active_idx > 0) {
                term_send_pos( map[active_field].row,
map[active_field].col + active_idx - 1);
                write(STDOUT_FILENO," ",1);
                active_idx--;
                term_send_pos( map[active_field].row,
                               map[active_field].col + active_idx);

            }

            break; 

        case ('\t'):       // Tab == 9
        case(ARROW_DOWN):
            term_send_cmd(HIDE_CURSOR);
            while(1) {
                active_field++;
                if ((map[active_field].attrb & UNPROT) == UNPROT) {
                    term_send_pos( map[active_field].row,
                                   map[active_field].col);
                    term_send_cmd(SHOW_CURSOR);
                    active_idx = 0;
                    break;
                }
                if (map[active_field].row == SENTINEL) {
                    active_field = 0;
                }
            }
        case(ARROW_RIGHT): break;
        case(ARROW_LEFT):break;

        case('\r'): return 9900;
        }
    }
}

void DSP_SEND(FIELD *map,input *cbook, int nElmnts) {
    char blanks[32];
    memset(blanks,' ', 32);
    for (int i = 0; i < nElmnts; i++) {
        term_send_pos(map[i].row,map[i].col);
        term_send_col(map[i].color);
        if((map[i].attrb & PROT) == PROT) {
            term_send_str(map[i].initial,map[i].len);
        } else {
            term_send_attr(UNDERLINE);
            term_send_str(blanks, map[i].len);
        }
        term_send_cmd(TERM_RESET);
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
    return;
}

