struct client_session {
    u32 client_id;
    u32 session_token;
    time_t login_time;
    time_t expiry;
    enum screen_state screen_state;
};

struct client_session_table {
    struct session sessions[500];
    int len;
};
