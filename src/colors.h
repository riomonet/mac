#ifndef COLORS_H
#define COLORS_H

enum colors {
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
    [MAGENTA] = { .name = "MAGENTA", .fg = "\x1b[38;2;98;65;180m", .fgLen = 18,.bg = "\x1b[48;2;255;00;255m", .bgLen = 18 },
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

#endif
