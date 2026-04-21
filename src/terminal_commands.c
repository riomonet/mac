void term_send_cmd(enum term_commands cmd) {
    char *seq = term_cmds[cmd].escape_seq;
    int len = term_cmds[cmd].len;
    write(STDOUT_FILENO, seq, len);
}

void term_send_pos(int y, int x) {
    char buf[32];
    snprintf(buf,32,"\x1b[%d;%dH",y,x);
    write(STDOUT_FILENO, buf, strlen(buf));
}

// NOTE: maybe change this storage in feildmap and cp to a byte;
void term_send_col(enum colors col) {
    char *color = colors[col].fg;
    int len =colors[col].fgLen;
    write(STDOUT_FILENO, color, len);
}

void term_send_str(char *str, int len) {
    write(STDOUT_FILENO, str,len);
}

void term_send_hlite (char dsp_attr) {
    int len = 4;
    if (dsp_attr & UNDERLINE) write(STDOUT_FILENO, "\x1b[4m", len );  
    if (dsp_attr & INVERSE)   write(STDOUT_FILENO, "\x1b[7m", len );
    if (dsp_attr & BLINK)     write(STDOUT_FILENO, "\x1b[5m", len );
    if (dsp_attr & STRIKE)    write(STDOUT_FILENO, "\x1b[9m", len );
}

void term_send_intensity(char dsp_attr) {
    int len = 4;
    if (dsp_attr & BOLD) write(STDOUT_FILENO, "\x1b[1m", len );
    if (dsp_attr & FAINT) write(STDOUT_FILENO, "\x1b[2m", len );
    if (dsp_attr & ITALIC) write(STDOUT_FILENO, "\x1b[3m", len );
}
