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
    [1] = { .name = "red",     .fg = "\033[31m", .bg = "\033[41m", .len = 5 },
    [2] = { .name = "green",   .fg = "\033[32m", .bg = "\033[42m", .len = 5 },
    [3] = { .name = "brown",   .fg = "\033[33m", .bg = "\033[43m", .len = 5 },
    [4] = { .name = "blue",    .fg = "\033[34m", .bg = "\033[44m", .len = 5 },
    [5] = { .name = "magenta", .fg = "\033[35m", .bg = "\033[45m", .len = 5 },
    [6] = { .name = "cyan",    .fg = "\033[36m", .bg = "\033[46m", .len = 5 },
    [7] = { .name = "white",   .fg = "\033[37m", .bg = "\033[47m", .len = 5 }
};
