#ifndef ATTR_H
#define ATTR_H

enum DSP_ATTR {
    NONE      =  0,
    UNDERLINE =  1u,
    BLINK     =  1u << 1,
    INVERSE   =  1u << 2,
    STRIKE    =  1U << 3,
    //bits 4 and 5 reserved for future use;
    BOLD      =  0b01000000,
    FAINT     =  0b10000000,
    ITALIC    =  0b11000000
};

enum FLD_ATTR {
    PROT      =  1u,
    IC        =  1u << 1,
    NUMERIC   =  1u << 2,
    HIDDEN    =  1u << 3,
    MDT       =  1u << 4,
    PERSIST   =  1u << 5,
};

#endif


