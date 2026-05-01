
global field fieldmap_login[] = {
    DMS( 8, 9,  27,  "USER . . . . . . . . . . . ", PROT, NONE, GREEN,""),
    DMS( 8, 38, 16,"",IC, UNDERLINE, GREEN, "user"),
    DMS( 10, 9,  27, "PASSWORD . . . . . . . . . ", PROT, NONE, GREEN, ""),
    DMS( 10, 38, 16, "",0, UNDERLINE, GREEN, "password"),
    DMS( 1, 40, 19, "Marina 59 | Sign On", PROT, NONE, WHITE, ""),
    DMS( 5, 5,  37, "Tab to change fields, Enter to submit", PROT, NONE, BLUE,""),
};

// TODO: Finding the index of named fields and number of fields better not manual.

global field fieldmap_mac[] = {
    DMS(1, 7, AUTO_DSP_USER, "DSP_USER", PROT, NONE, WHITE, ""),
    DMS(1,67, AUTO_DSP_DATE, "DSP_DATE", PROT, NONE, WHITE, ""),
    DMS(2, 67, AUTO_DSP_TIME, "DSP_TIME", PROT, NONE, WHITE, ""),
    DMS(24,6,2,"",IC,NONE,GREEN,"selection"), // TODO find idx by names.
    DMS(1,  0, 5,  "User:", PROT, NONE, WHITE, ""),
    DMS(1, 29, 21, "Marina Access Control", PROT, NONE, WHITE, ""),
    DMS(2, 35,  9, "MAIN MENU", PROT, NONE, WHITE,""),
    DMS(6,  6, 28, "Select one of the following:",PROT, NONE, BLUE, ""),
    DMS(8,10,15,   "1. Add customer",PROT, NONE,GREEN, ""),
    DMS(9,10,17,   "2. View customers",PROT, NONE,GREEN, ""),
    DMS(10,10,17,  "3. Access history",PROT, NONE,GREEN, ""),
    DMS(11,10,15,  "4. Live montior",PROT, NONE,GREEN, ""),
    DMS(23,0,9,"Selection",PROT,NONE,GREEN,""),
    DMS(24,0,4, "===>",PROT,NONE,GREEN,""),
    DMS(26,0,100,"DSP_HL",PROT,UNDERLINE,GREEN,""),
    DMS(28,6,9,"F6=Logout",PROT,NONE,BLUE,""),  
    DMS(28,19,9, "F7=Search",PROT,NONE,BLUE,""),
    DMS(28,31,16,"F8=Redraw screen",PROT,NONE,BLUE,""),
    DMS(29,0,100,"DSP_HL",PROT,UNDERLINE,GREEN,""),
};


/* global field  fieldmap_client_add[] = { */
/*     DMS(1,  7, 16, "", PROT, NONE,WHITE, "user"), */
/*     DMS(1, 67, 10, "", PROT,NONE,WHITE,"date"), //TODO: change to named field */
/*     DMS(2, 67,  8, "",PROT, NONE, WHITE, "time"), //TODO: change to names fieldx */
/*     DMS(1,  0, 5, "User:", PROT, NONE, WHITE, NULL), */
/*     DMS(1, 29, 21, "Marina Access Control", PROT, NONE, WHITE, NULL), */
/* }; */


