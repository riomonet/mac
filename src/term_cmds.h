#ifndef TERM_CMDS_H
#define TERM_CMDS_H

enum term_commands {
    CLEAR_SCREEN,
    SHOW_CURSOR,
    HIDE_CURSOR,
    ALT_BUFFER,
    ORIG_BUFFER,
    TERM_RESET,
    NOWRAP,
    WRAP
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
    [TERM_RESET] = { .name = "RESET", .escape_seq = "\x1b[0m", .len = 4 },
    [NOWRAP] = {.name = "NOWRAP", .escape_seq = "\x1b[?7l", .len = 5},
    [WRAP] = {.name = "NOWRAP", .escape_seq = "\x1b[?7h", .len = 5}
    
};


#endif
