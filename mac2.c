#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include "lookup_tables.h"
#include "term_interop.c"
#include "mac.h"
#include "menus.c"

#define GET_IDX(g, y, x) (((y) * ((g)->nCols)) + (x))

int inFrame(grid *g, int y, int x) {
  if (x < g->nCols && y < g->nRows){
    return 1;
  } else {
    return 0;
  }
}
  
void bltFrameBuffer(frameBuffer *frameBuf) {
    write(STDOUT_FILENO,frameBuf->data,frameBuf->len);
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

int getIdx(grid *g,int y, int x) {
  return (y * g->nCols + x);
}

void setAttribute(grid *g, int y, int x, enum attributes attr) {
  if(inFrame(g,y,x)) {
      int idx = GET_IDX(g,y,x);
      g->cell[idx].attr = attr;
  }
}

int xCenter(grid *g, int strlen) {
    return (g->nCols - strlen)/2;
}

int xThird(grid *g, int strlen) {
    return (g->nCols - strlen)/3;
}

/* Returns the total number of bytes in 'grid'. */
#if  0
size_t countBytes(grid *g) {
    int len = g->nCols * g->nRows;
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += 1;
	sum += g->cell[i].cursor.seqLen;
        sum += g->cell[i].fg.len;
        sum += g->cell[i].bg.len;
        sum += g->cell[i].attributes.len;
    }
    return sum;
}
#endif

int diffCells(cell *back, cell *front) {
  return ((back->column != front->column) ||
          (back->row != front->row) ||
          (back->char_code != front->char_code) ||
          (back->fgColor != front->fgColor) ||
          (back->bgColor != front->bgColor) ||
          (back->attr != front->attr));
}


/* Flattens 'grid' where each cell contains various string escape
 * sequence directives that specify the foreground color, background
 * color, and any special attributes like reverse or underline for the
 * character val of the cell into a flat 'frameBuffer' to write. */
void serializeGrid(grid *b, grid *f, frameBuffer *fb) {
    char *fbPtr = fb->data;
    memset(fb->data, 0, 1024 * 1024);
    fb->len = 0;
     for(int y = 0; y < b->nRows; y++) {
        for(int x = 0; x < b->nCols; x++) {
          int idx = GET_IDX(b,y,x);
          cell *back =  &b->cell[idx];
          cell *front = &f->cell[idx];
          if (diffCells(back, front))
              {
                char pos[32];
                snprintf(pos,32,"\x1b[%d;%dH",back->column, back->row);
                int len = strlen(pos);
                memcpy(fbPtr, pos, len);
                fbPtr+=len;

                memcpy(fbPtr,
                       attribute[back->attr].seqOn,
                       attribute[back->attr].len);
                fbPtr+=attribute[back->attr].len;
                
                memcpy(fbPtr,
                       colors[back->bgColor].bg,
                       colors[back->bgColor].bgLen);
                fbPtr += colors[back->bgColor].bgLen;

                memcpy(fbPtr,
                       colors[back->bgColor].fg,
                       colors[back->bgColor].fgLen);
                fbPtr+=colors[back->bgColor].fgLen;
                
                *fbPtr = back->char_code;
                fbPtr++;
              }
        }
    }
    fb->len = fbPtr - fb->data;
}


/* Allocates memory one time at the beginning for the framebuffer.
 * This memory block is reused every frame.*/
frameBuffer *initFrameBuffer(void) {
    frameBuffer *fb = malloc(sizeof(*fb) + (1024 * 1024));
    return fb;
}

/* Initialized the grid position of every 'cell' in 'grid'.
 * The escape sequence is written out for every cell prior to the
 * color, or character to be. */
void initCellPosition(grid *g) {
    for(int y = 0; y < g->nRows; y++) {
        for(int x = 0; x < g->nCols; x++) {
          
            /* cell *curCell = &(g->cell[y * g->nCols + x]); */
            /* char tmp[32]; */
            /* snprintf(tmp, 32,"\x1b[%d;%dH",y+1,x+1); */
            /* int len = strlen(tmp); */
            /* memcpy(curCell->cursor.pos, tmp, len); */
            /* curCell->cursor.seqLen = len; */
        }
    }
}

grid *allocateGrid(int nCols, int nRows) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * nCols * nRows));
    g->nRows = nRows;
    g->nCols = nCols;
    return g;
}

/* Sets the cursor position on the terminal */
void term_send_pos(int y, int x) {
    printf("\x1b[%d;%dH",y,x); //TODO: Convert to write call, using snprintf;
    fflush(stdout);
}

/* Sets the char value of each cell in grid 'g' to 'chVal', as
 * as well as the background and foreground colors if the
 * value passed is not NULL */
void resetGrid(grid *g, char char_code) {

  for(int y = 0; y < g->nRows; y++) {
    for(int x = 0; x < g->nCols; x++) {
      setChar(g,y,x,char_code);
      setFgCl(g,y,x,DEF_FG);
      setBgCl(g,y,x,DEF_BG);
      setAttribute(g,y,x,RESET);
      g->cell[GET_IDX(g,y,x)].column = y + 1;
      g->cell[GET_IDX(g,y,x)].row = x + 1;
    }
  }
}

/* Callback for 'SIGWINCH' signal */
void handler(int code) {
    if (code) RESIZE = 1;
}

void resizeGrid(grid *back, grid *front,  struct termConfig *E) {
    free(back);
    free(front);
    initTerm(E);
    back  = allocateGrid(E->nCols, E->nRows);
    front = allocateGrid(E->nCols, E->nRows);
    resetGrid(back, ' ');
    resetGrid(front, ' ');
}

/* Typesets 'txt' string horizontally to pos y, x on the terminal. With
 * Bg color bg and foreground color fg */
void hText(grid *g, char  *txt, int y, int x,
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

int writeToGrid(grid *g, int(*screen)(grid *g)) {
    return screen(g);
}

void setDefaultColors(enum colors bg, enum colors fg) {
    DEF_BG = bg;
    DEF_FG = fg;
    //    TODO generalize from decimal rgb to hex rgb
    printf("\x1b]11;rgb:00/00/aa\e\\");
    fflush(stdout);
}

void zeroFront(grid *g) {
    for(int y = 0; y < g->nRows; y++) {
      for(int x = 0; x < g->nCols; x++) {
        setChar(g,y,x,-1);
      }
    }
}

int main(void) {
    /* ============================== SIGNAL HANDLING  ======================================== */
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);    
    sa.sa_flags = SA_RESTART; // Restart interrupted sys-calls.
    sa.sa_handler = handler;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
	perror("sigaction"); 
    }
    
    term_send_cmd(ALT_BUFFER);
    term_send_cmd(CLEAR_SCREEN);
    term_send_cmd(HIDE_CURSOR);
    setDefaultColors(BLUE,WHITE);
    
    initTerm();
    grid *front = allocateGrid(E.nCols, E.nRows);
    grid *back = allocateGrid(E.nCols, E.nRows);
    grid *tmp;
    //    resetGrid(front,' ');
    zeroFront(front);
    resetGrid(back,' ');
    frameBuffer *fb = initFrameBuffer();
/* ============================== MAIN GAME LOOP  ======================================== */
    while(1) {
	if (RESIZE) {
	    RESIZE = 0;
	    free(back);
	    initTerm(E);
	    back  = allocateGrid(E.nCols, E.nRows);
	    front = allocateGrid(E.nCols, E.nRows);

        ///////	    resizeGrid(back, front, &E);
	}

	// TODO: switch statement here to dispatch screens
    resetGrid(back,' ');
    writeToGrid(back, loginScreen);
	serializeGrid(back, front, fb);
	bltFrameBuffer(fb);
	tmp = back;
	back = front;
	front = tmp;
	term_send_pos(1,1);
    }
    
/* ============================== CLEAN UP ======================================== */
    term_send_cmd(ORIG_BUFFER);
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(TERM_RESET);
    return 0;
}
