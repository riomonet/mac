
//(setq c-basic-offset 4)
//(c-set-ofet 'case-label 4)
//https://vtp100.net/docs/vt100-ug/chapter3.html#ED

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <signal.h>

#define ABUF_INIT {NULL, 0}
#define CTRL_KEY(k) ((k) & 0x1f)

struct menuState {
    int len;
    char title[32];
    char menu[][32];
};

struct terminalState
{
    int cx,cy;
    int rowoff;
    int screenrows;
    int screencols;
    int numrows;
    termios orig_termios;
};

static terminalState E;

void die(const char *s)
{
    // clears screen in case of error as wellx
    write(STDOUT_FILENO, "\x1b[H\x1b[2J",7);
    
    perror(s);
    printf("\r");
    exit(1);
}

void disableRawMode()
{
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH,&E.orig_termios ) == -1)
    {
	die("tcgetattr");
    }
}

void enableRawMode()
{

    if (tcgetattr(STDIN_FILENO,&E.orig_termios) == -1)
    {
      die("tcgetaddr");
    }
    
    atexit(disableRawMode);

    termios raw = E.orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON |IEXTEN| ISIG);
    raw.c_cc[VMIN] = 0;           // min bytes to read prior to return
    raw.c_cc[VTIME] = 1;          // the max amount of time to wait for bytes 10ths of second
  
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
	die("tcsetattr");
    }
}


void clearScreen()
{
    write (STDOUT_FILENO, "\x1b[2J", 4);
    write (STDOUT_FILENO, "\x1b[40m", 5);//back ground black
}

void setWindowSize()
{
    struct winsize ws;
    
    ioctl(1, TIOCGWINSZ, &ws);
    E.screencols = ws.ws_col;
    E.screenrows = ws.ws_row;
}

void handleSigWinCh(int unused __attribute__((unused))) {
    /* Set new E.screenrows E.screencols */
    setWindowSize();
}

void initUI()
{
    E.cx = 0;
    E.cy = 0;
    E.numrows = 0;
    E.rowoff = 0;
    setWindowSize();

    signal(SIGWINCH, handleSigWinCh);
}


void setPos(int x, int y)
{
    char buf[32] = {};
    snprintf(buf,32,"\x1b[%d;%dH",x,y);
    write (STDOUT_FILENO,buf, strlen(buf));
}

int readKey()
{
    int nread;
    char c;
    nread = read(STDIN_FILENO, &c, 1);
    if(nread == -1 && errno != EAGAIN) die("read");
    return c;
}
    

int login()
{
    setPos(1, (E.screencols -17) / 2);
    write(STDOUT_FILENO,"Marina 59 | Login", 17);
    /* write(STDOUT_FILENO,"Login Name", 10); */
    /* write(STDOUT_FILENO,"Password", 8); */
    return 0;
}

int main()
{
  enableRawMode();
  initUI();
  clearScreen();  
  setPos(1,1);
  
  struct menuState mainMenu = {.menu = {"Login", "add user","view logs"}};

  printf("%s\n",mainMenu.menu[1]);

  while(1)
  {
      /* login(); */
      char c = readKey();

      /* for(int i = 1; i <= E.screenrows; ++i) */
      /* { */
      /*     for(int j= 1; j <= E.screencoljjjjjjjjjjj; ++j) */
      /*     { */
      /*         write(STDOUT_FILENO,"A", 1); */
      /*     } */
      /*     if (i != E.screenrows) write(STDOUT_FILENO,"\r\n",2); */
      /* } */
      
      /* usleep(500000); */

      /* for(int i = 1; i <= E.screenrows; ++i) */
      /* { */
      /*     for(int j= 1; j <= E.screencols; ++j) */
      /*     { */
      /*         write(STDOUT_FILENO,"Z", 1); */
      /*     } */
      /*     if (i != E.screenrows) write(STDOUT_FILENO,"\r\n",2); */
      /* } */


      
      /* he does processKeys which calls readKey */
            
      if (c == CTRL_KEY('q'))
      {
          write(STDOUT_FILENO, "\x1b[H",3);
          write(STDOUT_FILENO, "\x1b[2J",4);
          exit(0);
      }
  }
}
