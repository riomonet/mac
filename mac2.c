#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "lookup_tables.h"

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
    int width;
    int height;
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
    int idx = y * g->width + x;
    g->cell[idx].fg.color = colors[color].fg;
    g->cell[idx].fg.len = colors[color].len;
    g->cell[idx].nBytes += colors[color].len;
}
void setBgCl(grid *g, int y, int x, enum colors color) {
    int idx = y * g->width + x;
    g->cell[idx].bg.color = colors[color].bg;
    g->cell[idx].bg.len = colors[color].len;
    g->cell[idx].nBytes += colors[color].len;
}

void setChar(grid *g, int y, int x, char ch) {
    int idx = y * g->width + x;
    g->cell[idx].ch = ch;
}

void setAttb(grid *g, int y, int x) {
    int idx = y * g->width + x;
}

size_t countBytes(grid *g) {
    int len = g->width * g->height;
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
 * val of the cell */
frameBuffer *serializeGrid(grid *g) {
    size_t nBytes = countBytes(g);
    frameBuffer *fb = malloc(sizeof(*fb) + nBytes);
    fb->len = nBytes;
    char *fbPtr = fb->data;
    for(int i = 0; i < g->height; i++) {
        for(int j = 0; j < g->width; j++) {
            cell curCell = g->cell[i * g->width + j];
	    memcpy(fbPtr,curCell.bg.color,curCell.bg.len);
	    fbPtr+=curCell.bg.len;
	    memcpy(fbPtr,curCell.fg.color,curCell.fg.len);
	    fbPtr+=curCell.fg.len;
            *fbPtr++ = curCell.ch;
        }
    }
    return fb;
} 

grid *initGrid(int width, int height) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * width * height));
    g->width = width;
    g->height = height;
    for(int i = 0; i < g->height; i++) {
	for(int j = 0; j < g->width; j++) {
	    g->cell[i * g->width + j].nBytes = 1;
	}
    }
    return g;
}

/* Clears the terminal screen */
void term_send_clr() {
    printf("\x1b[2J");
    fflush(stdout);
}

/* Sets the cursor position on the terminal */
void term_send_pos(int y, int x) {
    printf("\x1b[%d;%dH",y,x);
    fflush(stdout);
}

/* Sets the char value of each cell in grid 'g' to 'chVal', as
 * as well as the background and foreground colors if the
 * value passed is not NULL */
void resetGrid(grid *g, char chVal, enum colors bgCl, enum colors fgCl) {
    for(int i = 0; i < g->height; i++) {
        for(int j = 0; j < g->width; j++) {
	    setBgCl(g,i,j,bgCl);
	    setFgCl(g,i,j,fgCl);
	    setChar(g,i,j,chVal);
	}
    }
}

int main(void) {
    term_send_clr();
    grid *g = initGrid(20,30);
    resetGrid(g,' ',blue, white);
    setChar(g,10,10,'A');
    setChar(g,10,11,'B');
    setChar(g,10,12,'C');
    frameBuffer *fb = serializeGrid(g);
    blitFrameBuffer(fb);
    term_send_pos(1,1);
    printf("\x1b[0m");
    return 0;
}
