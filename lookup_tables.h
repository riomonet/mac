
enum colors {
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    DEFAULT
};


struct color {
    char *name;
    char *fg;
    uint8_t fgLen;
    char *bg;
    uint8_t bgLen;
};

struct color colors[] = {
    [BLACK]   = { .name = "BLACK",   .fg = "\033[30m", .fgLen = 5,  .bg = "\033[40m", .bgLen = 5 },
    [RED]     = { .name = "RED",     .fg = "\x1b[38;2;170;0;0m",.fgLen = 15, .bg = "\x1b[48;2;170;0;0m", .bgLen = 15},
    [GREEN]   = { .name = "GREEN",   .fg = "\x1b[38;2;0;170;0m", .fgLen = 15,.bg = "\x1b[48;2;0;170;0m", .bgLen = 15},
    [BROWN]   = { .name = "BROWN",   .fg = "\x1b[38;2;170;85;0m", .fgLen = 16,.bg = "\x1b[48;2;170;85;0m", .bgLen = 16 },
    [BLUE]    = { .name = "BLUE",    .fg = "\x1b[38;2;0;0;170m", .fgLen = 15,.bg = "\x1b[48;2;0;0;170m", .bgLen = 15 },
    [MAGENTA] = { .name = "MAGENTA", .fg = "\x1b[38;2;255;85;255m", .fgLen = 18,.bg = "\x1b[48;2;255;85;255m", .bgLen = 18 },
    [CYAN]    = { .name = "CYAN",    .fg = "\x1b[38;2;85;255;255m", .fgLen = 18,.bg = "\x1b[48;2;85;255;255m", .bgLen = 18 },
    [WHITE]   = { .name = "WHITE",   .fg = "\033[97m", .fgLen = 5, .bg = "\033[107m", .bgLen = 6 }
};


enum term_commands {
    CLEAR_SCREEN,
    SHOW_CURSOR,
    HIDE_CURSOR,
    ALT_BUFFER,
    ORIG_BUFFER,
    RESET
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
    [RESET] = { .name = "RESET", .escape_seq = "\x1b[0m", .len = 4 },
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
    char *seq;
    uint8_t len;
};

enum attributes {
    BOLD,
    FAINT,
    ITALIC,
    UNDERLINE,
    BLINKING,
    INVERSE,
    HIDDEN,
    STRIKE
};

struct attr_obj attribute[] = {
    [BOLD] = {.name = "BOLD", .seq = "\x1b[1m", .len =4 },
    [FAINT] = {.name = "FAINT", .seq = "\x1b[2m", .len =4},
    [ITALIC] = {.name = "ITALIC", .seq = "\x1b[3m", .len =4 },
    [UNDERLINE] = {.name = "UNDERLINE", .seq = "\x1b[4m", .len = 4 },
    [BLINKING] = {.name = "BLINKING", .seq = "\x1b[5m", .len = 4},
    [INVERSE] = {.name = "INVERSE", . seq = "\x1b[7m", .len = 4},
    [HIDDEN] =  {.name = "HIDDEN", .seq = "\x1b[8m", .len = 4},
    [STRIKE] = {.name = "STRIKE", .seq = "\x1b[9m", .len = 4}
};

