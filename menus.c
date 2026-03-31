#define GET_IDX(g, y, x) (((y) * ((g)->nCols)) + (x))

typedef enum states {
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
} states;

typedef struct point {
    int row;
    int col;
} point;

typedef struct {
    states state;
    int cx, cy;

} userWindow;

userWindow W;

/* ===================================== Start up functions.  ==================================== */
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
void writeString(grid *g, point p, char *str, char *fmt, ...) {
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
    
    for (size_t i = 0; i < strlen(str); i++) {
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

/* ===================================== Point object caluclations  ==================================== */
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

/* ============================== Forms ============================== */
#define HALF .50
#define ONE_THIRD .33
#define INPUT_WIDTH 40
#define LABEL_WIDTH 28
#define DOUBLE_SPACE 2


typedef struct input {
    struct {
	char buf[32];
	point pt;
    } label;
    struct {
	char buf[16];
	point pt;
    } input;
} input;

typedef struct form {
    point base;
    int nFields;
    input field[8];
} form;

/* Constructor for 'input' type. */
input Input(char *label) {
    input inp = {.label.buf = ". . . . . . . . . . . . . . . . ",
		 .input.buf = "                "};
    memcpy(inp.label.buf, label, strlen(label));
    return inp;
}

/* Constructor for 'Form' type. */
form Form(char **inputs, int nFields, point basePt, int label_width, int line_space ) {
    form f;
    int rowStart = 1;
    for (int i = 0; i < nFields; i++) {
	f.field[i] = Input(inputs[i]);
	f.field[i].label.pt.row = basePt.row + rowStart;
	f.field[i].label.pt.col = basePt.col;
	f.field[i].input.pt.row = basePt.row + rowStart;
	f.field[i].input.pt.col = basePt.col + label_width;
	rowStart += line_space;
    }
    f.nFields = nFields;
    return f;
 }

void renderTitle(grid *g, char *title) {
    writeString(g, _Pt(g,strlen(title), 1, HALF), title, 0);
}

void renderForm(grid *g, form f) {
    for (int i = 0; i < f.nFields; i++) {
	writeString(g,f.field[i].label.pt, f.field[i].label.buf, 0);
	writeString(g,f.field[i].input.pt, f.field[i].input.buf, "a", UNDERLINE);
    }
}

static form loginForm = {.nFields = 0}; 

//TODO handle tab, backspace, and arrow keys, add submit F8 or something like that
void mac_handleInput () {
    form *f;
    if (W.state == LOGIN) f = &loginForm;
    term_send_pos(W.cy + 1, W.cx + 1);
    term_send_cmd(SHOW_CURSOR);
    char c = platform_read();
    int inputCol = f-> field[0].input.pt.col;
    int lastCol = inputCol + 16 - 1; // where do we get 16 from ?
    int curIdx = W.cx - inputCol;
    if ((curIdx + inputCol) < lastCol && W.cx - inputCol >= 0 ) {
	f->field[0].input.buf[curIdx] = c;
	W.cx++;
    }
}

void login(grid *g) {
    if (!loginForm.nFields) {
	char *fields[] = {"User", "Password"};
	point basePt = _Pt(g, INPUT_WIDTH, ONE_THIRD, HALF);
	loginForm = Form(fields, 2, basePt, LABEL_WIDTH, DOUBLE_SPACE);
	W.cx = loginForm.field[0].input.pt.col;
	W.cy = loginForm.field[0].input.pt.row;
    }
    renderTitle(g, "MARINA 59 | Sign On");
    renderForm(g, loginForm);
}

/* ============================== Serices provided to the platform layer ============================== */
void mac_renderWindow(grid *g) {
    switch(W.state) {
    case LOGIN:
	W.state = LOGIN;
	login(g);
	break;
    case MAIN_MENU:
    case ADD_USER:
    case VIEW_LIVE_LOGS:
    case SEARCH_LOGS: break;
    }
}

void mac_startup() {
    W.state = LOGIN;
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    setDefaultColors(BLACK,AMBER);
}
#if 0
void mac_handleInput(grid *g, char c) {
    int row = window.row;
    int col = window.col;
    ////   setCellState(g,1,1,"c",c);
    
}
#endif
