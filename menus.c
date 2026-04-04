#define GET_IDX(g, y, x) (((y) * ((g)->nCols)) + (x))


/* ===================================== Start up functions. =========================== */

/* On start we set the default colors for the ap. */
void setDefaultColors(enum colors bg, enum colors fg) {
    DEF_BG = bg;
    DEF_FG = fg;

    char *color = rgb_colors[bg].rgb;
    int len = rgb_colors[bg].len;
    write(STDOUT_FILENO, color, len);
}

/* ===================================== Utility functions.  ==================================== */
/* Returns true if y, x are within the visible window */
int inFrame(grid *g, int y, int x) {
    if (x < g->nCols && y < g->nRows){
        return 1;
    } else {
        return 0;
    }
}

/* ===================================== Cell values.  ========================================== */

void setFgCl(grid *g, int y, int x, enum colors color) {
    if(inFrame(g,y,x)) {
        int idx = GET_IDX(g,y,x);
        g->cell[idx].fgColor = color;
    }
}

void setBgCl(grid *g, int y, int x, enum colors color) {
    if(inFrame(g,y,x)) {
        int idx = GET_IDX(g,y,x);
        g->cell[idx].bgColor = color;
    }
}

void setChar(grid *g, int y, int x, char ch) {
    if(inFrame(g,y,x)) {
        int idx = GET_IDX(g,y,x);
        g->cell[idx].char_code = ch;
    }
}

void setAttribute(grid *g, int y, int x, enum attributes attr) {
    if(inFrame(g,y,x)) {
        int idx = GET_IDX(g,y,x);
        g->cell[idx].attr = attr;
    }
}

void setCellState(grid *g, int row, int col, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    while(*fmt) {
        switch(*fmt) {
        case 'c':
            char c = va_arg(args, int);
            setChar(g, row, col, c);
            break;
        case 'f':
            enum colors fg = va_arg(args, enum colors);
            setFgCl(g,row,col,fg);
            break;
        case 'b':
            enum colors bg = va_arg(args, enum colors);
            setBgCl(g,row,col,bg);
            break;
        case 'a':
            enum attributes attr = va_arg(args, enum attributes);
            setAttribute(g,row, col,attr);
            break;
        default:
            fprintf(stdout,"Bad format in setCellState");
            exit(1);
        }
        fmt++;
    }
    va_end(args);
}

/* Sets all the values for each column in a row starting at 
 * row 'y' and column 'x' for the length of 'str'. Can optionally
 * set colors and attributes. If 'fmt' string is 0. colors and attributes
 * are ignored. other wise we must include f->foreground, b->background
 * and a->attributes in the 'fmt' string
 *
 *
 *  TODO If string is wider than screen print only up to screen
 * width  we are getting segfaults. */
void writeString(grid *g, point p, char *str, int len, char *fmt, ...) {
    enum colors fg = DEFAULT;
    enum colors bg = DEFAULT;
    enum attributes attr = NONE;

    if (fmt) {
        va_list args;
        va_start(args, fmt);
        while(*fmt) {
            if (*fmt == 'f') fg = va_arg(args,enum colors);
            else if (*fmt == 'b') bg = va_arg(args, enum colors);
            else if (*fmt == 'a') attr = va_arg(args, enum attributes);
            fmt++;
        }
        va_end(args);
    }
    
    int y = p.row;
    int x = p.col;
    
    for (int i = 0; i < len; i++) {
        setChar(g,y,x+i,str[i]);
        if (bg) {
            setBgCl(g,y,x+i,bg);
        }
        if (fg) {
            setFgCl(g,y,x+i,fg);
        }
        if (attr) {
            setAttribute(g, y, x + i, attr);
        }
    }
}
void writeNum(grid *g, point p, int num, char *str, char *fmt, ...){
    char buf[32];
    snprintf(buf,32,"%s %d", str, num);
    writeString(g,p,buf,strlen(buf),fmt);
}
/* =============================== Point operations. ======================== */
/* Returns a column value given a percentage of nCols */
int _Col(grid *g, int len,float pos) {
    if (pos < 1) {
        return (g->nCols - len) * pos;
    }
    else {
        return pos;	
    }
}

/* Returns a row value given a percentage of nRows */
int _Row(grid *g, float pos) {
    if(pos < 1) {
        return g->nRows * pos;
    } else {
        return pos;
    }
}

/* Point constructor by percentage of width and height of screen.
 * If len is not null, the point uses, centers a string  of len
 * 'len' at the given proportion, horizontally */
point _Pt(grid *g, int len, float rwRatio, float clRatio) {
    point p = {
        .row = _Row(g, rwRatio),
        .col = _Col(g, len, clRatio)
    };
    return p;
}

point ptAddPt(point p, point q) {
    point pt =  {
        .row = p.row + q.row,
        .col = p.col + q.col
    };
    return pt;
}

/* Return a new point incrementing arg 'p' by nRows and nCols */
point ptAdd (point p, int nRows, int nCols) {
    point pt =  {
        .row = p.row + nRows,
        .col = p.col + nCols
    };
    return pt;
}

/* Point constructor. */
point Point(float row, float col) {
    point p = {.row = row, .col = col};
    return p;
}

/* Abreviated 'point' contstructor Pt. */
point Pt(float row, float col) {
    point p = {.row = row, .col = col};
    return p;
}

/* ================ Constructors for menus, forms, ... =============== */

void updateFieldMap(form *f, int nFields) {
    if (f->basePt.row < 0) f->basePt.row = 0;
    if (f->basePt.col < 0) f->basePt.col = 0;
    
    for (int i = 0; i < nFields; i++) {
        f->map[i].label = ptAddPt(f->basePt,f->offsets[i].label);
        f->map[i].entry = ptAddPt(f->basePt,f->offsets[i].entry);
    }
    f->curCol = f->map[0].entry.col + f->curIdx;
    f->curRow = f->map[f->curField].entry.row;
}

/* Return a 'field', which consists of a label and an entry. */
field Field(char *label) {
    field  f = {.label = ". . . . . . . . . . . . . . . . ",
                 .entry = "                "};
    memcpy (f.label, label, strlen(label));
    return f;
}

fieldMap FieldMap(point labelPt,point entryPt ){
    fieldMap m = {.label = labelPt, .entry = entryPt};
	return m;
}

/* 'form' constructor */
form Form(char **fields, int nFields, point basePt, fieldMap *offsets) {
    form f;
    f.curIdx = 0;
    f.curField = 0;
    f.basePt = basePt;
    f.nFields = nFields;
    
    for (int i = 0; i < nFields; i++) {
        f.field[i] = Field(fields[i]);
        f.offsets[i].label = offsets[i].label;
        f.offsets[i].entry = offsets[i].entry;
    }

    f.curCol = f.basePt.col + f.offsets[0].entry.col;
    f.curRow = f.basePt.row;
    f.nFields = nFields;
    updateFieldMap(&f, nFields);
    f.field[USER].entry[15] = 0; //NOTE This a hack!!!
    return f;
}


/* ================ Function for rendering components to the grid  =============== */

void renderTitle(grid *g, char *title) {
    writeString(g, _Pt(g,strlen(title), 1, HALF), title, strlen(title),"f", WHITE);
}

void renderInstructions(grid *g, char *inst) {
    writeString(g ,_Pt(g,strlen(inst),ONE_5TH,ONE_5TH),inst,strlen(inst), "f", MAGENTA);
}

void renderForm(grid *g, form *f, point basePt) {
    if (f->basePt.row != basePt.row || f->basePt.col != basePt.col) {
	f->basePt = basePt;
	updateFieldMap(f, f->nFields);
	E.cx = f->curCol;
	E.cy = f->curRow;
    }
    for (int i = 0; i < f->nFields; i++) {
        writeString(g,f->map[i].label, f->field[i].label, f->offsets[i].entry.col, 0);
        writeString(g,f->map[i].entry, f->field[i].entry, 16, "a", UNDERLINE);
    }
}



//TODO handle tab, backspace, and arrow keys, and add a submit
int mac_handleInput () {
    form *f;
    //    if (current_state == LOGIN) f = &loginForm;
    f = &Forms[LOGIN];
    if (f->basePt.row > 0) {
        int c = platform_read();

        // TODO: Replace hard coded field entry length.
        int lastCol = f->map[0].entry.col + 16 - 1; 
        if (isalnum(c) && E.cx < lastCol) {
            f->curCol++;
            f->field[f->curField].entry[f->curIdx] = c;
            f->curIdx++;
        } else if (c == 127 && f->curIdx > 0) { /* backspace */
            f->curIdx--;
            f->curCol--;
            f->field[f->curField].entry[f->curIdx] = ' ';
        } else if (c == 9) { /* TAB */
            f->curField = (f->curField + 1) % f->nFields;
            f->curRow = f->map[f->curField].entry.row;
            f->curCol = f->map[f->curField].entry.col;
            f->curIdx = 0;
        } else if(c == ARROW_DOWN) {
            f->curField = (f->curField + 1) % f->nFields;
            f->curRow = f->map[f->curField].entry.row;
            f->curCol = f->map[f->curField].entry.col;
            f->curIdx = 0;
        } else if(c == ARROW_RIGHT && E.cx < lastCol)  {
            f->curCol++;
            f->curIdx++;
        } else if (c == ARROW_LEFT && f->curIdx > 0)  {
            f->curCol--;
            f->curIdx--;
    }
        E.cx = f->curCol;
        E.cy = f->curRow;
        return c;
    }
    return 0;
}

int auth(form *f) {
    // NOTE: For now keeping entry padding for auth comparison. 
    char *user = f->field[USER].entry;
    char *pw = f->field[PASSWORD].entry;

    return ( (!strncmp("OfficerLogan   ", user, 15) &&
             (!strncmp("Password       ", pw, 15))));
}

point extractBase(grid *g, base_point bp) {
    return _Pt(g,bp.len,bp.yGeometry,bp.xGeometry);
}

//Form(char **fields, int nFields, point basePt, fieldMap *offsets) {
void initializeFormGenerationProcedure() {
    point pt = Pt(-1,-1);
    for(int STATE = 0; STATE < SENTINEL; STATE++) {
	Forms[STATE] = Form(fields[STATE], nFields[STATE], pt, offsets[STATE]);
    }
}

/* Render screen depending on global variabl 'current_state'. */
 void screenDispatch(grid *g) {
    int c = mac_handleInput();
    renderTitle(g, titles[current_state]);
    renderForm(g,&Forms[current_state], BASE_PT(g,current_state));

    //renderFooterMenu(Forms[current_state]); TBD
    //TODO: move to lookuptables
    //renderInstructions(g,"Press ENTER to submit credentials:");

    if (E.cy != Forms[current_state].map[Forms[current_state].curField].entry.row + 1) {
	E.cy = Forms[current_state].map[Forms[current_state].curField].entry.row + 1;
    }
}
/* ============================== Serices provided to the platform layer ============================== */
void mac_renderWindow(grid *g) {
    switch(current_state) {
    case LOGIN:
        screenDispatch(g);
        break;
    case MAC:
	screenDispatch(g);
	break;
    /* case ADD_USER: */
    /* case VIEW_LIVE_LOGS: */
    /* case SEARCH_LOGS: break; */
    default: break;
    }
}

void mac_startup() {
    current_state = LOGIN;
    setDefaultColors(BLACK,GREEN);
    initializeFormGenerationProcedure();
}
