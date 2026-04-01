static enum colors DEF_BG;
static enum colors DEF_FG;


#if 0
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
#endif

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


typedef enum states {
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
} states;




typedef struct {
    states state;
    int cx, cy;
} userWindow;

userWindow W;


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

char platform_read(); 
