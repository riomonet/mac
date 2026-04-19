int main_menu () {
    
    /* struct date_time dt = date_today(); */
    /* char *fields[] = {"user", "date", "time"}; */
    /* char *vals [] =  {current_operator, dt.date, dt.time}; */
    /* set_cb_output(cb, fields, vals, 3); */
    //   int ic =  DSP_SEND(fieldmap_mac, cb);
    int ic =  DSP_SEND(fieldmap_mac);
    int res = DSP_RECIEVE(fieldmap_mac, LEN_FIELDMAP_MAC, ic);
    if (res == ENTER) {
        return CLIENT;
    }
    return 0;
}
