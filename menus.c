
/* ===================================== Individual cell and grid functions.  ==================================== */

#define GET_IDX(g, y, x) (((y) * ((g)->nCols)) + (x))

int inFrame(grid *g, int y, int x) {
  if (x < g->nCols && y < g->nRows){
    return 1;
  } else {
    return 0;
  }
}

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


/* TODO combing the following 2 functions into single variadic
 * with some kind of nomenclature specifying what we want. expand for thirds and fifths
 * horizontally and vertically haves/quarters/thirds/fifths, maybe golden ratio HA! */
int xCenter(grid *g, int strlen) {
    return (g->nCols - strlen)/2;
}

int xThird(grid *g, int strlen) {
    return (g->nCols - strlen)/3;
}

/* Sets all the values for each column in a row starting at 
 * row 'y' and column 'x' for the length of 'txt'. */
void writeString(grid *g, int y, int x, char *str, char *fmt, ...) {

    va_list args;
    va_start(args, fmt);
    enum colors fg = DEFAULT;
    enum colors bg = DEFAULT;
    enum attributes attr = NONE;

    while(*fmt) {
	if (*fmt == 'f') fg = va_arg(args,enum colors);
	else if (*fmt == 'b') bg = va_arg(args, enum colors);
	else if (*fmt == 'a') attr = va_arg(args, enum attributes);
    }

    va_end(args);
    
    for (int i = 0; i < (int)strlen(str); i++) {
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

void setDefaultColors(enum colors bg, enum colors fg) {
    DEF_BG = bg;
    DEF_FG = fg;
    printf("\x1b]11;rgb:00/00/aa\e\\");
    fflush(stdout);
}

// NOTE We have to keep track of where we are.
// TODO put these in a menus.h file
typedef enum states {
    STARTUP,
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
} states;

typedef struct {
    states state;
    int row;
    int col;
} userWindow;

userWindow window;
/* Pre game loop directives, service provided by mac layer to platform */
void mac_startup() {
    window.state = STARTUP;
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    setDefaultColors(BLUE,WHITE);
}

/* ============================== Screen rendering and application state functionality. ============================== */

/* NOTE: What other kinds of items are there, 'Input', 'Menu', 'DataRow' ? */
typedef struct ui_Item {
    char item[48];
    int len;
    int row;
    int col;
} ui_Item;

typedef struct ui_Form {
    int numItems;
    ui_Item *items;
} ui_Form;

ui_Item createInput_Label(char *label) {
    ui_Item f = {0};
    char frame[] = ". . . . . . . . . . . . . . . . ";
    memcpy(f.item, frame, strlen(frame));
    memcpy(f.item, label, strlen(label));
    return f;
}

ui_Item createReadBlock() {
    ui_Item f = {0};
    strcpy(f.item, "                ");
}

ui_Item createInput(char *label, int row, int col) {
    ui_Item user = createInput_Lable("User");
    ui_Item textArea = createReadBlock();
    ui_Item input;
    return input;
}

#if 0
void renderLoginForm (grid *g, char *title, inputItem *inputs) {
  horText(g, title, 1, xCenter(g,strlen(title)), DEFAULT,DEFAULT,NONE);
  int x = xCenter(g , strlen(inputs[0].label) + strlen(inputs[0].input));
  horText(g, inputs[0].label, 12, x + 5  , DEFAULT, DEFAULT, BOLD);
  horText(g, inputs[0].input, 12, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
  horText(g, inputs[1].label, 14, x + 5  , DEFAULT, DEFAULT, BOLD);
  horText(g, inputs[1].input, 14, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
}
#endif

void renderLoginScreen(grid *g) {
    char *title = "MARINA 59 | Sign On";
    writeString(g, 12, 5, title, "");
    uiItem user = createInput("User");
    uiItem password = createInput("Password");
    
    term_send_pos(user.row, user.col);
    term_send_pos(password.row, password.col);
    term_send_cmd(SHOW_CURSOR);
}

/* Mac service provided to platform. */
void mac_renderWindow(grid *g) {
    switch(window.state) {
    case STARTUP:
	renderLoginScreen(g);
	window.state = LOGIN;
	break;
    case LOGIN: renderLoginScreen(g); break;
    case MAIN_MENU:
    case ADD_USER:
    case VIEW_LIVE_LOGS:
    case SEARCH_LOGS: break;
    }
}

void mac_handleInput(grid *g, char c) {
    int row = window.row;
    int col = window.col;
    setCellState(g,row,col,"c", c);
    window.row++;
    putchar(c);
}
