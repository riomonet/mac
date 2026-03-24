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

typedef struct form {
    char *title;
    int numFields;
    inputField fields[];
} form;

inputField createInputField(char *label) {
    inputField f = {0};
    char frame[] = ". . . . . . . . . . . .";
    memcpy(f.label, frame, strlen(frame));
    memcpy(f.label, label, strlen(label));
    return f;
}




/* ============================== SCREENS SECTIONS  ======================================== */
int loginScreen(grid *g) {
    //    char *title = "MARINA 59 | Sign On";
    inputField user = createInputField("User");
    //hText(g, title, 1, xCenter(g,strlen(title)), DEFAULT,DEFAULT);
    hText(g, user.label, 16 , 20 , DEFAULT, DEFAULT);
    //    hText(g, password, 17, xThird(g,strlen(password)), DEFAULT, DEFAULT);
    return 1;
}
