#ifndef ATTR_H
#define ATTR_H

struct attr_obj {
    char *name;
    char *seqOn;
    char *seqOff;
    uint8_t len;
};

//redundant
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

enum highlights {
    UNDERLINE,
    BLINKING,
    INVERSE,
    STRIKE
}
    
enum ext_attr {
    _NONE = 0,
    EXT_BOLD = 1, //attrib
    EXT_FAINT = 2,
    EXT_ITALIC = 4,
    EXT_UNDERLINE = 8,  // highlite
    EXT_BLINKING = 16, // highlite
    EXT_INVERSE = 32,  // hihghlight
    EXT_HIDDEN = 64,  // attrb
    EXT_STRIKE = 128, // highlight
};

// these are for fieldmaps
enum t_attrb{
    PROT    = 0b00000001,
    IC      = 0b00000010,
    MDT     = 0b00000100,
    NUMERIC = 0b00001000,
    INTESE  = 0b00010000
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

#endif
