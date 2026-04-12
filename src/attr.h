#ifndef ATTR_H
#define ATTR_H

enum attributes {
    HIDDEN ,
    NUMERIC,
};

enum intensity {
    NORMAL = 0b00,
    BOLD   = 0b01,
    FAINT  = 0b10,
    ITALIC = 0b11
};
enum hlite {
    NONE      =  0b000,
    UNDERLINE =  0b001,
    BLINKING  =  0b010,
    INVERSE   =  0b100,
    STRIKE    =  0b101
};
    
// these are for fieldmaps
enum t_attrb{
    PROT    = 0b01,
    IC      = 0b10,
    MDT     = 0x80
};

#endif
