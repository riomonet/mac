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


/* Once per frame, the framebuffer is written to the terminal */
void bltFrameBuffer(frameBuffer *frameBuf) {
    write(STDOUT_FILENO,frameBuf->data,frameBuf->len);
}

/*  Returns true (any non zero) if there is any difference between corresponding
 *  cells in the front and back buffers. */
int diffCells(cell *back, cell *front) {
  return ((back->column != front->column) ||
          (back->row != front->row) ||
          (back->char_code != front->char_code) ||
          (back->fgColor != front->fgColor) ||
          (back->bgColor != front->bgColor) ||
          (back->attr != front->attr));
}

/* If there is any differnece between corresponding front and back buffer 
 * cells, that cells member string directive information (escape sequences and
 * 'char_code') are looked up in loookup.h and inserted into the frameBuffer 
 * 'data' member. The frameBuffer 'len' is incremented accordingly. */
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

                // background color
                memcpy(fbPtr,
                       colors[back->bgColor].bg,
                       colors[back->bgColor].bgLen);
                fbPtr += colors[back->bgColor].bgLen;

                //foreground color
                memcpy(fbPtr,
                       colors[back->fgColor].fg,
                       colors[back->fgColor].fgLen);
                fbPtr+=colors[back->fgColor].fgLen;
                
                *fbPtr = back->char_code;
                fbPtr++;
              }
        }
    }
    fb->len = fbPtr - fb->data;
}

/* Platform layer: Allocates memory one time at the 
 * beginning for the framebuffer.  This memory block is
 * reused every frame.*/
frameBuffer *initFrameBuffer(void) {
    frameBuffer *fb = malloc(sizeof(*fb) + (1024 * 1024));
    return fb;
}

/* ??? which layer */
grid *allocateGrid(int nCols, int nRows) {
    grid *g = malloc(sizeof(*g) + (sizeof(cell) * nCols * nRows));
    g->nRows = nRows;
    g->nCols = nCols;
    return g;
}

/* service provided to mac*/
void term_send_pos(int y, int x) {
    printf("\x1b[%d;%dH",y,x); //TODO: Convert to write call, using snprintf;
    fflush(stdout);
}

/* platform layerSets the char value of each cell in grid 'g' to 'chVal', as
 * as well as colors and attributess */
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

/* Platform layer*/
void signalHandler(int code) {
    switch (code) {
    case SIGWINCH: RESIZE = 1; break;
    case SIGINT: CLEANUP = 1; break;
    }
}


/* platform layer */
void resizeGrid(grid *back, grid *front,  struct termConfig *E) {
    free(back);
    free(front);
    initTerm(E);
    back  = allocateGrid(E->nCols, E->nRows);
    front = allocateGrid(E->nCols, E->nRows);
    resetGrid(back, ' ');
    resetGrid(front, ' ');
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
    sa.sa_handler = signalHandler;
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
        perror("sigaction"); 
    }
    // TOOD(ari): This should only be active during debug mode.
    // otherwise SIGINT should be ignored once in raw mode.
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("SIGINT");
    }
    term_send_cmd(ALT_BUFFER);
    enableRawMode();
    mac_startup();
    
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
        if (CLEANUP) {
            goto CLEAN_UP;
        }
        if (RESIZE) { // TODO(ari): move this to function _resize.
            RESIZE = 0;
            free(back);
            initTerm(E);
            back  = allocateGrid(E.nCols, E.nRows);
            front = allocateGrid(E.nCols, E.nRows);
            ///////	    resizeGrid(back, front, &E);
        }
        resetGrid(back,' ');
	char c;
	read(STDIN_FILENO, &c, 1);
	mac_handleInput(back, c);
        mac_renderWindow(back); // TODO(ari): throttle frame rate in platform
        serializeGrid(back, front, fb);
        bltFrameBuffer(fb);
        tmp = back;
        back = front;
        front = tmp;
    }
    /* ============================== CLEAN UP ======================================== */

 CLEAN_UP:
    term_send_cmd(ORIG_BUFFER);
    term_send_cmd(SHOW_CURSOR);
    term_send_cmd(TERM_RESET);
    printf("\x1b]11;rgb:00/00/00\e\\"); // TODO currently this is Reset to Black, need to query original state.
    fflush(stdout);
    return 0;
}
