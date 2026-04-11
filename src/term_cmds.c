void term_send_cmd(enum term_commands cmd) {
    char *seq = term_cmds[cmd].escape_seq;
    int len = term_cmds[cmd].len;
    write(STDOUT_FILENO, seq, len);
}

/* void term_send_write(char *seq, int len) { */
/*     write(STDOUT_FILENO, seq, len); */
/* } */

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

void term_send_ext_attr(enum ext_attr attr) {
    if (attr & EXT_BOLD)
        term_send_attr(BOLD);
    if (attr & EXT_FAINT)
        term_send_attr(FAINT);
    if (attr & EXT_ITALIC)
        term_send_attr(ITALIC);
    if (attr & EXT_UNDERLINE)
        term_send_attr(UNDERLINE);
    if (attr & EXT_BLINKING)
        term_send_attr(BLINKING);
    if (attr & EXT_INVERSE)
        term_send_attr(INVERSE);
    if (attr & EXT_HIDDEN)
        term_send_attr(HIDDEN);
    if (attr & EXT_STRIKE)
        term_send_attr(STRIKE);
}

