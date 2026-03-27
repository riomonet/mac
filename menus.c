
/* ===================================== Individual cell and grid functions.  ==================================== */

#define GET_IDX(g, y, x) (((y) * ((g)->nCols)) + (x))

int inFrame(grid *g, int y, int x) {
  if (x < g->nCols && y < g->nRows){
    return 1;
  } else {
    return 0;
  }
}


//TODO combine the following 4 functions into a single variadic function
// with a "cfba" char string specifying what should be set.
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
void horText(grid *g, char  *txt, int y, int x,
           enum colors bg, enum colors fg, enum attributes attr) {
    int i;
    for (i = 0; i < (int)strlen(txt); i++) {
        setChar(g,y,x+i,txt[i]);
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
enum states {
    STARTUP,
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
};

typedef struct current_state {
    enum states current;
} current_state;

current_state State;
/* Pre game loop directives, service provided by mac layer to platform */
void mac_startup() {
    State.current = STARTUP;
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    setDefaultColors(BLUE,WHITE);
}

/* ============================== Screen rendering and application state functionality. ============================== */

/* NOTE: What other kinds of items are there, 'Input', 'Menu', 'DataRow' ? */
typedef struct inputItem {
    char label[32];
    char input[24];
} inputItem;

inputItem createInputItem(char *label) {
    inputItem f = {0};
    char frame[] = ". . . . . . . . . . . . . . . . ";
    char input[] = "               ";
    memcpy(f.label, frame, strlen(frame));
    memcpy(f.label, label, strlen(label));
    memcpy(f.input, input, strlen(input));
    return f;
}

void renderLoginForm (grid *g, char *title, inputItem *inputs) {
  horText(g, title, 1, xCenter(g,strlen(title)), DEFAULT,DEFAULT,NONE);
  int x = xCenter(g , strlen(inputs[0].label) + strlen(inputs[0].input));
  horText(g, inputs[0].label, 12, x + 5  , DEFAULT, DEFAULT, BOLD);
  horText(g, inputs[0].input, 12, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
  horText(g, inputs[1].label, 14, x + 5  , DEFAULT, DEFAULT, BOLD);
  horText(g, inputs[1].input, 14, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
  term_send_pos(13, x + 30);

}

void renderLoginScreen(grid *g) {
    char *title = "MARINA 59 | Sign On";
    inputItem inputs[] = {
	createInputItem("User"),
	createInputItem("Password"),
    };
    renderLoginForm(g, title, inputs);
}

/* Mac service provided to platform. */
void mac_writeToGrid(grid *g) {
    switch(State.current) {
    case STARTUP:
	renderLoginScreen(g);
	State.current = LOGIN;
	break;
    case LOGIN: renderLoginScreen(g); break;
    case MAIN_MENU:
    case ADD_USER:
    case VIEW_LIVE_LOGS:
    case SEARCH_LOGS: break;
    }
}

void mac_handleInput(grid *g, char c) {
    setChar(g,1,1,c);
}
