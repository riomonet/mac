int main_menu () {
    /* struct date_time dt = date_today(); */

    init_mac_fieldmap_cb();    
    int ic =  DSP_SEND(fieldmap_mac, MAC_NUM_FIELDS);
    int res = DSP_RECIEVE(fieldmap_mac, MAC_NUM_FIELDS, ic);
    /* if (res == ENTER) { */
    /* } */
    return 1;
}
