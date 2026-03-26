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

}

int loginScreen(grid *g) {
    char *title = "MARINA 59 | Sign On";
    inputField fields[2];
    fields[0] = createInputField("User");
    fields[1] = createInputField("Password");
    printForm(g, title, fields);
    return 1;
}
