static enum colors DEF_BG;
static enum colors DEF_FG;

struct termConfig {
    int nRows; 			/* The number of rows in the terminal window */
    int nCols;			/* The number of columns in the terminal window. */
    int cx, cy; 		/* The current cursor postion. */
    struct termios orig_termios;
};

typedef struct cell {
  short column;
  short row;
  char char_code;
  enum colors fgColor;
  enum colors bgColor;
  enum attributes attr;
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

struct termConfig E;

grid *initGrid(int, int);
void resizeGrid(grid*,grid*,struct termConfig*);
void setCursorPostions(grid*);
void clearAllGridCells(grid*, char);
int writeToGrid(grid*, int(*screen)(grid *g));
size_t countBytes(grid*);

frameBuffer *initFrameBuffer(void);
void serializeGrid(grid*, grid*, frameBuffer*);
void bltFrameBuffer(frameBuffer*);

void setFgCl(grid*, int, int, enum colors);
void setBgCl(grid*, int, int, enum colors);
void setChar(grid*, int, int, char);
void underline(grid*, int, int, int, enum colors, enum colors);
void handler(int);
void setDefaultColors(enum colors, enum colors);
void term_send_pos(int, int);

int platform_read(); 
