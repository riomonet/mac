enum colors {
    black, red, green, brown, blue, magenta, cyan, white
};

struct color {
    char *name;
    char *fg;
    char *bg;
    uint8_t len;
};

struct color colors[8] = {
    [0] = { .name = "black",   .fg = "\033[30m", .bg = "\033[40m", .len = 5 },
    [1] = { .name = "red",     .fg = "\x1b[38;2;170;0;0m", .bg = "\x1b[48;2;170;0;0m", .len = 15},
    [2] = { .name = "green",   .fg = "\x1b[38;2;0;170;0m", .bg = "\x1b[48;2;0;170;0m", .len = 15},
    [3] = { .name = "brown",   .fg = "\x1b[38;2;170;85;0m", .bg = "\x1b[48;2;170;85;0m", .len = 16 },
    [4] = { .name = "blue",    .fg = "\x1b[38;2;0;0;170m", .bg = "\x1b[48;2;0;0;170m", .len = 15 },
    [5] = { .name = "magenta", .fg = "\x1b[38;2;255;85;255m", .bg = "\x1b[48;2;255;85;255m", .len = 18 },
    [6] = { .name = "cyan",    .fg = "\x1b[38;2;85;255;255m", .bg = "\x1b[48;2;85;255;255m", .len = 18 },
    [7] = { .name = "white",   .fg = "\033[47m", .bg = "\033[47m", .len = 5 }
};


/* void term_send_attrib(attributes attrib) { */
/*     switch(attrib) { */
/*     case reset: printf("\x1b[0m"); fflush(stdout); return; */
/*     case bold: printf("\x1b[1m"); fflush(stdout); return; */
/*     case underline: printf("\x1b[4m"); fflush(stdout); return; */
/*     case blink: printf("\x1b[5m"); fflush(stdout); return; */
/*     case reverse: printf("\x1b[7m"); fflush(stdout); return; */
/*     default: */
/*     } */
/* } */
