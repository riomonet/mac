#define PT(r, c) {.row = r, .col = c}
#define FMP(rl, cl, re, ce) { .label = { .row = rl, .col = cl },  .entry = { .row = re, .col = ce } }

typedef struct point {
    int row;
    int col;
} point;

typedef struct fieldMap {
    point label;
    point entry;
} fieldMap;

typedef enum states {
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
} states;

states current_state;

enum components {
    LOGIN_FORM,
    MASTER_MENU
};

fieldMap login_offsets[] = { FMP(0,0,0,25), FMP(2,0,2,25) };


fieldMap *offsets[] = {
    [LOGIN_FORM] = login_offsets
};    


point loginBase = _Pt(g ,FIELD_LEN, ONE_3RD, HALF); 
point basePts[] = {
        [LOGIN_FORM] = 
        
};
