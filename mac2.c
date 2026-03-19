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
    g->cell[idx].nBytes += 1;
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

/* Turn 'grid' where each cell is a variable number of string into a
 *  flat buffer */
frameBuffer *serializeGrid(grid *g) {
    size_t nBytes = countBytes(g);
    frameBuffer *fb = malloc(sizeof(*fb) + nBytes);
    char *fbPtr = fb->data;
    for(int i = 0; i < g->height; i++) {
        for(int j = 0; j < g->width; j++) {
            cell curCell = g->cell[i * g->width + j];
            *fbPtr++ = curCell.ch;
        }
    }
} 

grid *initGrid(int width, int height) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * width * height));
    g->width = width;
    g->height = height;
    return g;
}

int main(void) {
    grid *g = initGrid(20,30);
    setChar(g,10,10,'A');
    setChar(g,10,11,'B');
    setChar(g,10,12,'C');

    return 0;
}
