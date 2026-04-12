void term_send_cmd(enum term_commands cmd) {
    char *seq = term_cmds[cmd].escape_seq;
    int len = term_cmds[cmd].len;
    write(STDOUT_FILENO, seq, len);
}

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

void term_send_intensity(enum intensity inty) {
    char *seq = NULL;
    int len = 4;
    switch (inty) {
    case BOLD:
        seq = "\x1b[1m";
        break;
    case FAINT:
        seq = "\x1b[2m";
        break;
    case ITALIC:
        seq = "\x1b[3m";
        break;
    default: break;
    }
    if (seq) {
        write(STDOUT_FILENO, seq, len );
    }
}

void term_send_str(char *str, int len) {
    write(STDOUT_FILENO, str,len);
}


void term_send_hlite (enum hlite hl) {
    char *seq = NULL;
    int len = 4;
    switch (hl) {
    case UNDERLINE:
        seq = "\x1b[4m";
        break;
    case INVERSE:
        seq = "\x1b[7m";
        break;
    case BLINKING:
        seq = "\x1b[5m";
        break;
    case STRIKE: break;
        seq = "\x1b[9m";
        break;
    default: break;
    }
    if (seq) {
        write(STDOUT_FILENO, seq, len );
    }
}



/* void term_unpack_(enum ext_attr attr) { */
/*     if (attr & EXT_BOLD) */
/*         term_send_attr(BOLD); */
/*     if (attr & EXT_FAINT) */
/*         term_send_attr(FAINT); */
/*     if (attr & EXT_ITALIC) */
/*         term_send_attr(ITALIC); */
/*     if (attr & EXT_UNDERLINE) */
/*         term_send_attr(UNDERLINE); */
/*     if (attr & EXT_BLINKING) */
/*         term_send_attr(BLINKING); */
/*     if (attr & EXT_INVERSE) */
/*         term_send_attr(INVERSE); */
/*     if (attr & EXT_HIDDEN) */
/*         term_send_attr(HIDDEN); */
/*     if (attr & EXT_STRIKE) */
/*         term_send_attr(STRIKE); */
/* } */

