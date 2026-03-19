/* (setq c-basic-offset 4) */
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios orig;

void raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig);
    struct  termios raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void restore_mode(void)
{
    tcsetattr(STDIN_FILENO,TCSAFLUSH, &orig);
}

#define MAX_GUESTS 10000

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct String
{
    u8 len;
    char buf[31];
};

struct user
{
    String First;
    String Last;
    String Email;
    String Phone;
    String Psk;
    uint32_t Id;
};


struct Guest
{
    int nextGuest;
    Entity Data[10];
};

Guest Guests;
void addField(String *Data, String *Field)
{
    memcpy(Field, Data, sizeof(String));
}

enum Screens
{
    S_Login,
    S_MainMenu,
    S_AddUser,
    S_ViewUsers,
    S_FindUser,
    S_ViewLogs,
    S_Exit
};

void Prompt(const char*, String*);
String GetInput();
void cls(void);

Screens F_AddUser(void);
Screens F_MainMenu(void);
Screens F_Login(void);
Screens F_ViewUsers(void);
Screens F_FindUser(void);
Screens F_ViewLogs(void);

typedef enum Screens (*sp)(void);
sp ScreenDispatch[] =
{
    [S_Login] = F_Login,
    [S_MainMenu] = F_MainMenu,
    [S_AddUser] = F_AddUser,
    [S_ViewUsers] = F_ViewUsers,
    [S_FindUser] = F_FindUser,
    [S_ViewLogs] = F_ViewLogs,
};

int main(int argc, char* argv[])
{
    raw_mode();
    int quit = 0;
    Screens Nxt = S_MainMenu;

    printf(ESC"m33 ARI");
    /* while (!quit) */
    /* { */
    /*     Nxt = ScreenDispatch[Nxt](); */
    /*     if (Nxt == S_Exit) */
    /*         { */
    /*             cls(); */
    /*             restore_mode(); */
    /*             exit (0); */
    /*         } */
    /* } */
    return 0;
}

void MenuBanner(const char* Menu)
{
    printf("************\n %s\n************\n\n",Menu);
}

enum Screens F_MainMenu()
{

    MenuBanner("MAIN MENU");
        
    int row = 10, col = 20;
    cls();

    const char *items[] =
        {
            "Add new user",
            "View all users",
            "Find user",
            "view logs",
            "logout"
        };
    for(int i = 0; i< 5;i++)
        {
            printf("\033[33m");
            printf("\033[%d;%dH(%d) %s",row,col,i,items[i]);
            row++;
        }

    char ch = getchar();
 
    switch(ch)
    {
    case '1': return S_AddUser;
    case '2': return S_ViewUsers;
    case '3': return S_FindUser;
    case '4': return S_ViewLogs;
    case '5': return S_Login;
    case '6': return S_Exit;
    }
    return S_MainMenu;
}

enum Screens F_AddUser()
{
    String First = {},Last,Email,Phone;

    cls();
    MenuBanner("ADD USER");

    Prompt("First", &First);
    Prompt("Last",  &Last);
    Prompt("Email", &Email);
    Prompt("Phone", &Phone);
    
    return S_MainMenu;
}

enum Screens F_ViewUsers()
{
    return S_ViewUsers;
}
enum Screens F_FindUser()
{
    return S_FindUser;
}
enum Screens F_ViewLogs()
{



    return S_ViewLogs;
}
enum Screens F_Login()
{
    String Username, Psk;
    cls();
    Prompt("Username", &Username);
    cls();
    Prompt("Password", &Psk);
    //TODO:(ari) verify credentials
    //NOTE:(ari) will also need to limit logins, etc.
    // maybe can look at ssh code. 3 tries and time out for 1 minute
    // 12 tries time out 10 hours
    /* if verified return main menu */
    return S_MainMenu;
    /* else return S_Login*/
}

String GetInput()
{
    String tmp = {};
    char c = 0;
    for (int i = 0;c != '\n'; ++i)
    {
        c = getchar();
        tmp.buf[i] = c;
        tmp.len++;
    }
    return tmp;
}

void Prompt(const char *label, String *out)
{
    printf("%s: ", label);
    *out = GetInput();
}


void PrintStr(String str)
{
    for (int i = 0; i <= str.len; i ++)
        putc(str.buf[i],stdout);
}

void cls()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

void gotoxy(int r, int c)
{
    printf("\033[%d;%dH", r, c);
}
