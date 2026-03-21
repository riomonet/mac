#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "lookup_tables.h"
#include "term_interop.c"

typedef struct cell {
    size_t nBytes;
    char ch;
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
    g->cell[idx].nBytes += colors[color].fgLen;
}
void setBgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->nCols + x;
    g->cell[idx].bg.color = colors[color].bg;
    g->cell[idx].bg.len = colors[color].bgLen;
    g->cell[idx].nBytes += colors[color].bgLen;
}

void setChar(grid *g, int y, int x, char ch) {
    int idx = y * g->nCols + x;
    g->cell[idx].ch = ch;
}

void setAttb(grid *g, int y, int x) {
    int idx = y * g->nCols + x;
}

size_t countBytes(grid *g) {
    int len = g->nCols * g->nRows;
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += g->cell[i].nBytes;
    }
    return sum;
}

/* Returns a flatbuffer assembled from a 'grid' where each cell
 * contains various string escape sequence directives that specify
 * the foreground color, background color, and any special
 * attributes like reverse or underline for the character
 * val of the cell.
 *
 *
 * TODO: a big block should be allocated at startup and just
 * use that instead of calling malloc every time */
frameBuffer *serializeGrid(grid *g) {
    size_t nBytes = countBytes(g);
    frameBuffer *fb = malloc(sizeof(*fb) + nBytes);
    fb->len = nBytes;
    char *fbPtr = fb->data;
    for(int i = 0; i < g->nRows; i++) {
        for(int j = 0; j < g->nCols; j++) {
            cell curCell = g->cell[i * g->nCols + j]; 
	    //memcpy(fbPtr,curCell.bg.color,curCell.bg.len);
	    //fbPtr+=curCell.bg.len;  
	    //memcpy(fbPtr,curCell.fg.color,curCell.fg.len);
	    //fbPtr+=curCell.fg.len;
            *fbPtr++ = curCell.ch;
        }
    }
    return fb;
} 

grid *initGrid(int nCols, int nRows) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * nCols * nRows));
    g->nRows = nRows;
    g->nCols= nCols;
    for(int i = 0; i < g->nRows; i++) {
	for(int j = 0; j < g->nCols; j++) {
	    g->cell[i * g->nCols + j].nBytes = 1;
	}
    }
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
void resetGrid(grid *g, char chVal) {
    for(int i = 0; i < g->nRows; i++) {
        for(int j = 0; j < g->nCols; j++) {
	    setChar(g,i,j,chVal);
	}
    }
}

void handler(int code) {
    RESIZE = 1;
}

grid *resizeGrid(grid *gOld, struct termConfig *E) {
    free(gOld);
    initTerm(E);
    fflush(stdout);
    grid *g = initGrid(E->nCols, E->nRows);
    resetGrid(g, ' '); 
    return g;
}

void writeToGrid(grid *g) {
    setChar(g,10,10,'A');
    setChar(g,10,11,'B');
    setChar(g,10,12,'C');    
}

void setDefaultColors(enum colors bg, enum colors fg) {
    printf(colors[fg].fg);
    printf(colors[bg].bg);
    fflush(stdout);
}

int main(void) {
    term_send_cmd(CLEAR_SCREEN);
    setDefaultColors(BLUE,WHITE);

    struct sigaction sa = {0};
    /* memset(&sa, 0, sizeof(sa)); */ // Clear it to avoid garbage values
    sigemptyset(&sa.sa_mask);    // Don't block any other signals
    sa.sa_flags = SA_RESTART;    // Restart interrupted sys-calls
    sa.sa_handler = handler;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
	perror("sigaction");
    }
    term_send_cmd(HIDE_CURSOR);
    struct termConfig E;
    initTerm(&E);
    grid *g = initGrid(E.nCols, E.nRows);
    resetGrid(g,' ');
    writeToGrid(g);
    while(1) {
	if (RESIZE) {
	    RESIZE = 0;
	    g = resizeGrid(g, &E);
	    if( countBytes(g) != g->nRows * g->nCols) {
		printf("nRows * nCols: %d * %d = %d\n", g->nRows , g->nCols, countBytes(g));
		fflush(stdout);
	    }
	}
	writeToGrid(g);
	frameBuffer *fb = serializeGrid(g);
	blitFrameBuffer(fb);
	free(fb);
	term_send_pos(1,1);
    }

 CLEANUP:
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(RESET);
    return 0;
}
