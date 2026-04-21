int main_menu () {
    /* struct date_time dt = date_today(); */

    init_mac_fieldmap_cb();    
    int ic =  display_manager_send(fieldmap_mac, MAC_NUM_FIELDS);
    int res = display_manager_recieve(fieldmap_mac, MAC_NUM_FIELDS, ic);
    /* if (res == ENTER) { */
    /* } */
    return 1;
}
