static enum colors DEF_BG;
static enum colors DEF_FG;

typedef struct cell {
    char ch;
    struct {
        size_t seqLen;
        char pos[32]; //TODO make this 16 bytes. 
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

grid *initGrid(int, int);
void resizeGrid(grid*,grid*,struct termConfig*);
void setCursorPostions(grid*);
void clearAllGridCells(grid*, char);
int writeToGrid(grid*, int(*screen)(grid *g));
size_t countBytes(grid*);

frameBuffer *initFrameBuffer(void);
void serializeGrid(grid*, grid*, frameBuffer*);
void bltFrameBuffer(frameBuffer*);

int xCenter(grid*, int);
int xThird(grid*, int) ;
void setFgCl(grid*, int, int, enum colors);
void setBgCl(grid*, int, int, enum colors);
void setChar(grid*, int, int, char);
void underline(grid*, int, int, int, enum colors, enum colors);
void hText(grid*, char*, int, int, enum colors, enum colors, enum attributes);
void handler(int);
void setDefaultColors(enum colors, enum colors);
void term_send_pos(int, int);




