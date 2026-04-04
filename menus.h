#define PT(r, c) {.row = r, .col = c}
#define FMP(rl, cl, re, ce) { .label = { .row = rl, .col = cl },  .entry = { .row = re, .col = ce } }
#define BPT(l,yG,xG) {.len = l, .yGeometry = yG, .xGeometry = xG}
#define NUM_FIELDS(arr) (sizeof(arr) / sizeof((arr)[0]))

#define HALF .50
#define ONE_3RD .33
#define TWO_3RD .66
#define ONE_4TH .25
#define THREE_4TH .75
#define ONE_5TH .20
#define TWO_5TH .40
#define THREE_5TH .60
#define FOUR_5TH .80

#define FIELD_LEN 40
#define LABEL_LEN 25

#define USER 0
#define PASSWORD 1

typedef struct point {
    int row;
    int col;
} point;

typedef struct fieldMap {
    point label;
    point entry;
} fieldMap;

typedef enum states {
    LOGIN,
    MAC,
    //    ADD_USER,
    //    VIEW_LIVE_LOGS,
    //    SEARCH_LOGS,
    SENTINEL
} states;

typedef struct base_point {
    int len;
    float yGeometry;
    float xGeometry;
} base_point;

typedef struct field {
    char label[32];
    char entry[32];
} field;

typedef struct form {
    point basePt;
    int  nFields;
    int  curField;
    int  curRow;
    int  curCol;
    int  curIdx;
    struct fieldMap offsets[10]; // NOTE: Temporarily allocated here.
    struct fieldMap map[10]; // NOTE: actual is derived from offsets
    field field[10]; // NOTE Temporarily allocated here.
} form;

states current_state;

fieldMap offsets_login[] = { FMP(0,0,0,25), FMP(2,0,2,25) };
fieldMap offsets_mac[] = { FMP(0,0,0,0), FMP(2,0,0,0), FMP(4,0,0,0), FMP(6,0,0,0), FMP(8,0,0,0) };

char *fields_login[] = {"User", "Password"};
char *fields_mac[] = {"View Users", "Add User", "Logs View", "Live View"};

char title_login[] = "MARINA 59 | Sign On";
char title_mac[] =  "MARINA 59 | Access Control";

const base_point basePt_login = BPT(FIELD_LEN, ONE_3RD,HALF);
const base_point basePt_mac = BPT(20, ONE_5TH,ONE_5TH );

fieldMap *offsets[] = {
    [LOGIN] = offsets_login,
    [MAC] = offsets_mac
};

char **fields[] = {
    [LOGIN] = fields_login,
    [MAC] = fields_mac
};

char *titles[] = {
    [LOGIN] = title_login,
    [MAC] = title_mac
};

base_point basePoints[] = {
    [LOGIN] = basePt_login,
    [MAC] = basePt_mac
};

#define BASE_PT(g,state) _Pt(g,basePoints[state].len,basePoints[state].yGeometry,basePoints[state].xGeometry)

int nFields[] = {
    [LOGIN] = NUM_FIELDS(fields_login),
    [MAC] =  NUM_FIELDS(fields_mac)
};

//static form loginForm = {.nFields = 0, .curField = 0};
form Forms[128];





