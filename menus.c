enum SCREENS {
    LOGIN,
    MAIN_MENU,
    ADD_USER,
    VIEW_LIVE_LOGS,
    SEARCH_LOGS
};

typedef struct intputField {
    char label[24];
    char input[24];
} inputField;


inputField createInputField(char *label) {
    inputField f = {0};
    char frame[] = ". . . . . . . . . . . .";
    char input[] = "               ";
    memcpy(f.label, frame, strlen(frame));
    memcpy(f.label, label, strlen(label));
    memcpy(f.input, input, strlen(input));
    return f;
}

void printForm(grid *g, char *title, inputField *inputs, int len) {
    hText(g, title, 1, xCenter(g,strlen(title)), DEFAULT,DEFAULT,NONE);
    int i = 0;
    while(i  < len) {
        hText(g, inputs[i].label, 16 + i, 20 , DEFAULT, DEFAULT, NONE);
        hText(g, inputs[i].input, 16 + i, 40 , DEFAULT, DEFAULT, UNDERLINE);
        i++;
    }
}


int loginScreen(grid *g) {
    char *title = "MARINA 59 | Sign On";
    inputField fields[2];
    fields[0] = createInputField("User");
    fields[1] = createInputField("Password");
    printForm(g, title, fields, 2);
    return 1;
}
