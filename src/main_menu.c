int main_menu (struct copybook *cb_mac) {
    struct date_time dt = date_today();
    char *fields[] = {"user", "date", "time"};
    char *vals [] =  {current_operator, dt.date, dt.time};
    set_cb_output(cb_mac, fields, vals, 3);
    int ic =  DSP_SEND(fieldmap_mac, cb_mac);
    int res = DSP_RECIEVE(fieldmap_mac, cb_mac, ic);
    return 0;
}
