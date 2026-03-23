#include <stdio.h>
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


typedef struct cell {
    char ch;
    struct {
        size_t seqLen;
        char pos[32];
    } cursor;
    struct {
        size_t len;
        char *color;
    } fg;
    struct {
        size_t len;
        char *color;
    } bg;
    struct {
        size_t len;
        char *val;
    } attributes;
} cell;

typedef struct grid {
    int nCols;
    int nRows;
     cell cell[];
} grid;

typedef struct {
    int len;
    char data[];
} frameBuffer;

void blitFrameBuffer(frameBuffer *frameBuf) {
    write(STDOUT_FILENO,frameBuf->data,frameBuf->len);
}
void setFgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->nCols + x;
    g->cell[idx].fg.color = colors[color].fg;
    g->cell[idx].fg.len = colors[color].fgLen;
}

void setBgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->nCols + x;
    g->cell[idx].bg.color = colors[color].bg;
    g->cell[idx].bg.len = colors[color].bgLen;
}

void setChar(grid *g, int y, int x, char ch) {
     if (x < g->nCols && y < g->nRows) {
	g->cell[y * g->nCols + x].ch = ch;
    }
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

frameBuffer *initFrameBuffer(void) {
    frameBuffer *fb = malloc(sizeof(*fb) + (1024 * 1024));
    return fb;
}

grid *initGrid(int nCols, int nRows) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * nCols * nRows));
    g->nRows = nRows;
    g->nCols= nCols;
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
	    g->cell[i * g->nCols + j].bg.color = 0;
	    g->cell[i * g->nCols + j].bg.len = 0;
	    g->cell[i * g->nCols + j].fg.color = 0;
	    g->cell[i * g->nCols + j].fg.len = 0;
	}
    }
}

void setCursorPostions(grid *g) {
    for(int y = 0; y < g->nRows; y++) {
        for(int x = 0; x < g->nCols; x++) {
	    cell *curCell = &(g->cell[y * g->nCols + x]);
	    char tmp[32];
	    snprintf(tmp, 32,"\x1b[%d;%dH",y,x);
	    int len = strlen(tmp);
	    memcpy(curCell->cursor.pos, tmp, len);
	    curCell->cursor.seqLen = len;
	}
    }
}

void handler(int code) {
    if (code) RESIZE = 1;
}

grid *resizeGrid(grid *g, struct termConfig *E) {
    free(g);
    initTerm(E);
    g = initGrid(E->nCols, E->nRows);
    clearAllGridCells(g, ' ');
    setCursorPostions(g);
    return g;
}

void writeToGrid(grid *g) {
    setChar(g,10,90,'A');
    /* setBgCl(g,90,10, RED); */
    /* setChar(g,90,11,'B'); */
    /* setBgCl(g,90,11, GREEN);	 */
    /* setChar(g,90,12,'C'); */
    /* setBgCl(g,90,12, CYAN); */
    /* setFgCl(g,90,12,BLACK); */
    /* setBgCl(g,90,13, BLACK); */

}

void setDefaultColors(enum colors bg, enum colors fg) {
    printf("%s",colors[fg].fg);
    printf("%s",colors[bg].bg);
    fflush(stdout);
}

int main(void) {
    printf("\033[?1049h");
    fflush(stdout);
    term_send_cmd(CLEAR_SCREEN);
    setDefaultColors(BLUE,WHITE);

    /* Set up signal handling for change of window size. */
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);    
    sa.sa_flags = SA_RESTART; // Restart interrupted sys-calls.
    sa.sa_handler = handler;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
	perror("sigaction"); 
    }
    
    //    struct termConfig E;
    initTerm();
    grid *g = initGrid(E.nCols, E.nRows);
    setCursorPostions(g);
    frameBuffer *fb = initFrameBuffer();
    clearAllGridCells(g,' ');

    /* temp */
    writeToGrid(g);

    term_send_cmd(HIDE_CURSOR);
    while(1) {
	if (RESIZE) {
	    RESIZE = 0;
	    g = resizeGrid(g, &E);
	}
	writeToGrid(g);
	serializeGrid(g,fb);
	blitFrameBuffer(fb);
	term_send_pos(1,1);
    }

/* ============================== CLEAN UP ======================================== */
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(RESET);
    return 0;
}
