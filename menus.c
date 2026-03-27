enum SCREENS {
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
};

typedef struct intputField {
    char label[32];
    char input[24];
} inputField;

int editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
      if(nread == -1 && errno != EAGAIN) {
	    die("read");
      }

    }
    return c;
}

    #if 0
    if (c == '\x1b') {
        char seq[3] = {};
        if (read(STDIN_FILENO, &seq[0] , 1) != 1) {
            return '\x1b';
        }
        if (read(STDIN_FILENO, &seq[1] , 1) != 1) {
                return '\x1b';
            }
        if (seq[0] == '[') {
	    if (seq[1] >= '0' && seq[1] <= '9') {		
            if (read(STDIN_FILENO, &seq[2], 1) != 1)
                {
                    return '\x1b';
                }
            if (seq[2] == '~')
                {
                    switch(seq[1])
                        {
                        case '1' : return HOME_KEY;
                        case '3' : return DEL_KEY; 
                        case '4' : return END_KEY;
                        case '5' : return PAGE_UP;
                        case '6' : return PAGE_DOWN;
                        case '7' : return HOME_KEY;
                        case '8' : return END_KEY;
                        }
                }
	    }
	    else
	    {
		switch (seq[1])
		{
		    case 'A': return  ARROW_UP;
		    case 'B': return  ARROW_DOWN;
		    case 'C': return  ARROW_RIGHT;
		    case 'D': return  ARROW_LEFT;
		    case 'E': return  HOME_KEY;
		    case 'F': return  END_KEY;
		}
	    }
	}
	else if(seq[0] == 'O')
	{
	    switch(seq[1])
	    {
		case 'H': return HOME_KEY;
		case 'F': return END_KEY;
	    }
	}
	return '\x1b';	
    }
    else
    {

    }


}
#endif
inputField createInputField(char *label) {
    inputField f = {0};
    char frame[] = ". . . . . . . . . . . . . . . . ";
    char input[] = "               ";
    memcpy(f.label, frame, strlen(frame));
    memcpy(f.label, label, strlen(label));
    memcpy(f.input, input, strlen(input));
    return f;
}

void printForm(grid *g, char *title, inputField *inputs) {
  hText(g, title, 1, xCenter(g,strlen(title)), DEFAULT,DEFAULT,NONE);
  int x = xCenter(g,strlen(inputs[0].label) + strlen(inputs[0].input));
  hText(g, inputs[0].label, 12, x + 5  , DEFAULT, DEFAULT, BOLD);
  hText(g, inputs[0].input, 12, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
  hText(g, inputs[1].label, 14, x + 5  , DEFAULT, DEFAULT, BOLD);
  hText(g, inputs[1].input, 14, x + 29 , DEFAULT, DEFAULT, UNDERLINE);
  term_send_pos(13, x + 30);
  term_send_cmd(SHOW_CURSOR);
  //  editorReadKey();

}

int loginScreen(grid *g) {
    char *title = "MARINA 59 | Sign On";
    inputField fields[2];
    fields[0] = createInputField("User");
    fields[1] = createInputField("Password");
    printForm(g, title, fields);
    return 1;
}
