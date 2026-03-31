enum colors {
    DEFAULT,
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    AMBER
};

struct color {
    char *name;
    char *fg;
    uint8_t fgLen;
    char *bg;
    uint8_t bgLen;
};

struct rgb_color {
    char *name;
    char *rgb;
    uint8_t len;
};

struct color colors[] = {
    [BLACK]   = { .name = "BLACK",   .fg = "\033[38;2;0;0;0m", .fgLen = 13,  .bg = "\033[48;2;0;0;0m", .bgLen = 13 },
    [RED]     = { .name = "RED",     .fg = "\x1b[38;2;170;0;0m",.fgLen = 15, .bg = "\x1b[48;2;170;0;0m", .bgLen = 15},
    [GREEN]   = { .name = "GREEN",   .fg = "\x1b[38;2;70;255;00m", .fgLen = 17,.bg = "\x1b[48;2;0;170;0m", .bgLen = 15},
    [BROWN]   = { .name = "BROWN",   .fg = "\x1b[38;2;170;85;0m", .fgLen = 16,.bg = "\x1b[48;2;170;85;0m", .bgLen = 16 },
    [BLUE]    = { .name = "BLUE",    .fg = "\x1b[38;2;0;0;170m", .fgLen = 15,.bg = "\x1b[48;2;0;0;170m", .bgLen = 15 },
    [MAGENTA] = { .name = "MAGENTA", .fg = "\x1b[38;2;255;85;255m", .fgLen = 18,.bg = "\x1b[48;2;255;85;255m", .bgLen = 18 },
    [CYAN]    = { .name = "CYAN",    .fg = "\x1b[38;2;85;255;255m", .fgLen = 18,.bg = "\x1b[48;2;85;255;255m", .bgLen = 18 },
    [WHITE]   = { .name = "WHITE",   .fg = "\033[97m", .fgLen = 5, .bg = "\033[107m", .bgLen = 6 },
    [AMBER]   = { .name = "AMBER",   .fg = "\033[38;2;255;172;0m", .fgLen = 17 , .bg = "\033[48;2;255;165;0m", .bgLen = 17}
};

struct rgb_color rgb_colors[] = {
    [BLACK]   = { .name = "BLACK",   .rgb = "\x1b]11;rgb:00/00/00\e\\", .len = 19 },
    [RED]     = { .name = "RED",     .rgb = "\x1b]11;rgb:aa/00/00\e\\", .len = 19 },
    [GREEN]   = { .name = "GREEN",   .rgb = "\x1b]11;rgb:00/aa/00\e\\", .len = 19 },
    [BROWN]   = { .name = "BROWN",   .rgb = "\x1b]11;rgb:aa/55/00\e\\", .len = 19 },
    [BLUE]    = { .name = "BLUE",    .rgb = "\x1b]11;rgb:00/00/aa\e\\", .len = 19 },
    [MAGENTA] = { .name = "MAGENTA", .rgb = "\x1b]11;rgb:ff/55/ff\e\\", .len = 19 },
    [CYAN]    = { .name = "CYAN",    .rgb = "\x1b]11;rgb:55/ff/ff\e\\", .len = 19 },
    [WHITE]   = { .name = "WHITE",   .rgb = "\x1b]11;rgb:ff/ff/ff\e\\", .len = 19 },
    /* [AMBER]   = { .name = "AMBER",   .rgb = "\x1b]11;rgb:ff/a5/00\e\\", .len = 19 }, */
    [AMBER]   = { .name = "AMBER",   .rgb = "\x1b]11;rgb:ff/b7/00\e\\", .len = 19 },
};

//amber new #ffb700 | 255;172;00// "\033[38;2;255;165;0m old
//green alt #4aff00 | 70;255;00 // "\x1b[38;2;96;255;96m" old


enum term_commands {
    CLEAR_SCREEN,
    SHOW_CURSOR,
    HIDE_CURSOR,
    ALT_BUFFER,
    ORIG_BUFFER,
    TERM_RESET
};
    
struct term_cmd_obj {
    char *name;
    char *escape_seq;
    int len;
};

struct term_cmd_obj term_cmds[] = {
    [CLEAR_SCREEN] = { .name = "CLEAR_SCREEN", .escape_seq = "\x1b[2J", .len = 4 },
    [SHOW_CURSOR] = { .name = "SHOW_CURSOR", .escape_seq = "\x1b[?25h", .len = 6 },
    [HIDE_CURSOR] = { .name = "HIDE_CURSOR", .escape_seq = "\x1b[?25l", .len = 6 },
    [ALT_BUFFER] = { .name = "ALT_BUFFER", .escape_seq = "\x1b[?1049h", .len = 8 },
    [ORIG_BUFFER] = { .name = "ORIG_BUFFER", .escape_seq = "\x1b[?1049l", .len = 8 },
    [TERM_RESET] = { .name = "RESET", .escape_seq = "\x1b[0m", .len = 4 }
};

void term_send_cmd(enum term_commands cmd) {
    char *seq = term_cmds[cmd].escape_seq;
    int len = term_cmds[cmd].len;
    write(STDOUT_FILENO, seq, len);
}

void term_send_write(char *seq, int len) {
    write(STDOUT_FILENO, seq, len);
}

struct attr_obj {
    char *name;
    char *seqOn;
    char *seqOff;
    uint8_t len;
};

enum attributes {
    NONE,
    BOLD,
    FAINT,
    ITALIC,
    UNDERLINE,
    BLINKING,
    INVERSE,
    HIDDEN,
    STRIKE,
    RESET
};

struct attr_obj attribute[] = {
    [BOLD] = {.name = "BOLD", .seqOn = "\x1b[1m", .seqOff = "\x1b[21m", .len =4 },
    [FAINT] = {.name = "FAINT", .seqOn = "\x1b[2m",.seqOff = "\x1b[22m", .len =4},
    [ITALIC] = {.name = "ITALIC", .seqOn = "\x1b[3m",.seqOff = "\x1b[23m", .len =4 },
    [UNDERLINE] = {.name = "UNDERLINE", .seqOn = "\x1b[4m", .seqOff = "\x1b[24m",.len = 4 },
    [BLINKING] = {.name = "BLINKING", .seqOn = "\x1b[5m", .seqOff = "\x1b[25m",.len = 4},
    [INVERSE] = {.name = "INVERSE", . seqOn = "\x1b[7m", .seqOff = "\x1b[27m",.len = 4},
    [HIDDEN] =  {.name = "HIDDEN", .seqOn = "\x1b[8m", .seqOff = "\x1b[28m",.len = 4},
    [STRIKE] = {.name = "STRIKE", .seqOn = "\x1b[9m", .seqOff = "\x1b[29m",.len = 4},
    [RESET] = {.name = "RESET", .seqOn = "\x1b[0m", .len = 4},
    [NONE] =  {.name = "NONE", .seqOn = "", .seqOff = "",.len = 0}
};

