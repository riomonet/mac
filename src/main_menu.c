int main_menu (struct copybook *cb) {
    
    struct date_time dt = date_today();
    char *fields[] = {"user", "date", "time"};
    char *vals [] =  {current_operator, dt.date, dt.time};
    set_cb_output(cb, fields, vals, 3);
    int ic =  DSP_SEND(fieldmap_mac, cb);
    int res = DSP_RECIEVE(fieldmap_mac, cb, ic);
    if (res == ENTER) {
	

    }
    return 0;
}
