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

void bltFrameBuffer(frameBuffer *frameBuf) {
    write(STDOUT_FILENO,frameBuf->data,frameBuf->len);
}

void setFgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->nCols + x;
    if (x < g->nCols && y < g->nRows) {
	g->cell[idx].fg.color = colors[color].fg;
	g->cell[idx].fg.len = colors[color].fgLen;
    }
}

void setBgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->nCols + x;
    if (x < g->nCols && y < g->nRows) {
	g->cell[idx].bg.color = colors[color].bg;
	g->cell[idx].bg.len = colors[color].bgLen;
    }
}

void setChar(grid *g, int y, int x, char ch) {
     if (x < g->nCols && y < g->nRows) {
	g->cell[y * g->nCols + x].ch = ch;
    }
}

int xCenter(grid *g, int strlen) {
    return (g->nCols - strlen)/2;
}

int xThird(grid *g, int strlen) {
    return (g->nCols - strlen)/3;
}

/* void setAttb(grid *g, int y, int x) { */
/*      int idx = y * g->nCols + x; */
/* } */

/* Returns the total number of bytes in 'grid'. */
size_t countBytes(grid *g) {
    int len = g->nCols * g->nRows;
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += 1;
	sum += g->cell[i].cursor.seqLen;
        sum += g->cell[i].fg.len;
        sum += g->cell[i].bg.len;
        //sum += g->cell[i].attributes.len;
    }
    return sum;
}

/* Flattens 'grid' where each cell contains various string escape
 * sequence directives that specify the foreground color, background
 * color, and any special attributes like reverse or underline for the
 * character val of the cell into a flat 'frameBuffer' to write. */
void serializeGrid(grid *g, frameBuffer *fb) {
    size_t nBytes = countBytes(g);
    fb->len = nBytes;
    char *fbPtr = fb->data;
    for(int i = 0; i < g->nRows; i++) {
        for(int j = 0; j < g->nCols; j++) {
            cell curCell = g->cell[i * g->nCols + j];
	    memcpy(fbPtr, curCell.cursor.pos, curCell.cursor.seqLen);
	    fbPtr+=curCell.cursor.seqLen;
	    if (curCell.bg.len > 0) {
		memcpy(fbPtr, curCell.bg.color, curCell.bg.len);
		fbPtr+=curCell.bg.len;
	    }
	    if (curCell.fg.len > 0) {
		memcpy(fbPtr, curCell.fg.color, curCell.fg.len);
		fbPtr+=curCell.fg.len;
	    }
            *fbPtr++ = curCell.ch;
        }
    }
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
void setCursorPostions(grid *g) {
    for(int y = 0; y < g->nRows; y++) {
        for(int x = 0; x < g->nCols; x++) {
	    cell *curCell = &(g->cell[y * g->nCols + x]);
	    char tmp[32];
	    snprintf(tmp, 32,"\x1b[%d;%dH",y+1,x+1);
	    int len = strlen(tmp);
	    memcpy(curCell->cursor.pos, tmp, len);
	    curCell->cursor.seqLen = len;
	}
    }
}

grid *initGrid(int nCols, int nRows) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * nCols * nRows));
    g->nRows = nRows;
    g->nCols= nCols;
    setCursorPostions(g);
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
void clearAllGridCells(grid *g, char chVal) {
    for(int i = 0; i < g->nRows; i++) {
        for(int j = 0; j < g->nCols; j++) {
	    setChar(g,i,j,chVal);
	    setFgCl(g,i,j,DEF_FG);
	    setBgCl(g,i,j,DEF_BG);
	}
    }
}

/* Callback for 'SIGWINCH' signal */
void handler(int code) {
    if (code) RESIZE = 1;
}

grid *resizeGrid(grid *g, struct termConfig *E) {
    free(g);
    initTerm(E);
    g = initGrid(E->nCols, E->nRows);
    clearAllGridCells(g, ' ');
    return g;
}

/* Typesets 'txt' string horizontally to pos y, x on the terminal. With
 * Bg color bg and foreground color fg */
void hText(grid *g, char  *txt, int y, int x, enum colors bg, enum colors fg) {
    size_t len = strlen(txt);
    for (int i = 0; i < (int)strlen(txt); i++) {
	setChar(g,y,x+i,txt[i]);
	if (bg != DEFAULT) {
	    setBgCl(g,y,x+i,bg);
	    setBgCl(g, y, x + len , DEF_BG);
	}
	if (fg != DEFAULT) {
	    setFgCl(g,y,x+i,fg);
	    setFgCl(g, y, x + len, DEF_FG);
	}
    }
}


int writeToGrid(grid *g, int(*screen)(grid *g)) {
    return screen(g);
}

void setDefaultColors(enum colors bg, enum colors fg) {
    DEF_BG = bg;
    DEF_FG = fg;
}

/* void underline(grid *g, int y, int x, int len, enum colors bg, enum colors fg) { */
/* } */

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
    grid *g = initGrid(E.nCols, E.nRows);
    frameBuffer *fb = initFrameBuffer();
    clearAllGridCells(g,' ');
/* ============================== MAIN GAME LOOP  ======================================== */
    while(1) {
	if (RESIZE) {
	    RESIZE = 0;
	    g = resizeGrid(g, &E);
	}
	// TODO: switch statement here to dispatch screens
	writeToGrid(g, loginScreen);
	serializeGrid(g,fb);
	bltFrameBuffer(fb);
	term_send_pos(1,1);
    }
/* ============================== CLEAN UP ======================================== */
    term_send_cmd(ORIG_BUFFER);
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(RESET);
    return 0;
}
