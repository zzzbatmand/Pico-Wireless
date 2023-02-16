#include "pico_wireless.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// picowireless Module
////////////////////////////////////////////////////////////////////////////////////////////////////

/***** Module Functions *****/
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_reset_obj, PicoWireless_reset);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_network_data_obj, PicoWireless_get_network_data);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_remote_data_obj, 2, PicoWireless_get_remote_data);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_network_obj, 2, PicoWireless_wifi_set_network);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_passphrase_obj, 3, PicoWireless_wifi_set_passphrase);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_key_obj, 4, PicoWireless_wifi_set_key);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_config_obj, 5, PicoWireless_config);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_set_dns_obj, 2, PicoWireless_set_dns);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_set_hostname_obj, 2, PicoWireless_set_hostname);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_disconnect_obj, PicoWireless_disconnect);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_connection_status_obj, PicoWireless_get_connection_status);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_mac_address_obj, PicoWireless_get_mac_address);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_ip_address_obj, PicoWireless_get_ip_address);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_subnet_mask_obj, PicoWireless_get_subnet_mask);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_gateway_ip_obj, PicoWireless_get_gateway_ip);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_current_ssid_obj, PicoWireless_get_current_ssid);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_current_bssid_obj, PicoWireless_get_current_bssid);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_current_rssi_obj, PicoWireless_get_current_rssi);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_current_encryption_type_obj, PicoWireless_get_current_encryption_type);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_start_scan_networks_obj, PicoWireless_start_scan_networks);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_scan_networks_obj, PicoWireless_get_scan_networks);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_ssid_networks_obj, 2, PicoWireless_get_ssid_networks);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_enc_type_networks_obj, 2, PicoWireless_get_enc_type_networks);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_bssid_networks_obj, 2, PicoWireless_get_bssid_networks);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_channel_networks_obj, 2, PicoWireless_get_channel_networks);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_rssi_networks_obj, 2, PicoWireless_get_rssi_networks);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_req_host_by_name_obj, 2, PicoWireless_req_host_by_name);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_host_by_name_obj, 1, PicoWireless_get_host_by_name);

MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_fw_version_obj, PicoWireless_get_fw_version);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_time_obj, PicoWireless_get_time);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_set_power_mode_obj, 2, PicoWireless_set_power_mode);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_ap_network_obj, 3, PicoWireless_wifi_set_ap_network);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_ap_passphrase_obj, 4, PicoWireless_wifi_set_ap_passphrase);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_ping_obj, 3, PicoWireless_ping);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_debug_obj, 2, PicoWireless_debug);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_temperature_obj, PicoWireless_get_temperature);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_pin_mode_obj, 3, PicoWireless_pin_mode);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_digital_write_obj, 3, PicoWireless_digital_write);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_analog_write_obj, 3, PicoWireless_analog_write);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_digital_read_obj, 2, PicoWireless_digital_read);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_analog_read_obj, 2, PicoWireless_analog_read);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_server_start_obj, 4, PicoWireless_server_start);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_client_start_obj, 5, PicoWireless_client_start);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_client_stop_obj, 2, PicoWireless_client_stop);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_server_state_obj, 2, PicoWireless_get_server_state);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_client_state_obj, 2, PicoWireless_get_client_state);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_avail_data_obj, 2, PicoWireless_avail_data);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_avail_server_obj, 2, PicoWireless_avail_server);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_data_obj, 3, PicoWireless_get_data);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_get_data_buf_obj, 2, PicoWireless_get_data_buf);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_insert_data_buf_obj, 3, PicoWireless_insert_data_buf);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_send_udp_data_obj, 2, PicoWireless_send_udp_data);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_send_data_obj, 3, PicoWireless_send_data);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_check_data_sent_obj, 2, PicoWireless_check_data_sent);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_get_socket_obj, PicoWireless_get_socket);

MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_ent_identity_obj, 2, PicoWireless_wifi_set_ent_identity);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_ent_username_obj, 2, PicoWireless_wifi_set_ent_username);
MP_DEFINE_CONST_FUN_OBJ_KW(PicoWireless_wifi_set_ent_password_obj, 2, PicoWireless_wifi_set_ent_password);
MP_DEFINE_CONST_FUN_OBJ_1(PicoWireless_wifi_set_ent_enable_obj, PicoWireless_wifi_set_ent_enable);

/***** Locals Table *****/
STATIC const mp_rom_map_elem_t PicoWireless_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&PicoWireless_reset_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_network_data), MP_ROM_PTR(&PicoWireless_get_network_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_remote_data), MP_ROM_PTR(&PicoWireless_get_remote_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_network), MP_ROM_PTR(&PicoWireless_wifi_set_network_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_passphrase), MP_ROM_PTR(&PicoWireless_wifi_set_passphrase_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_key), MP_ROM_PTR(&PicoWireless_wifi_set_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&PicoWireless_config_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_dns), MP_ROM_PTR(&PicoWireless_set_dns_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_hostname), MP_ROM_PTR(&PicoWireless_set_hostname_obj) },
    { MP_ROM_QSTR(MP_QSTR_disconnect), MP_ROM_PTR(&PicoWireless_disconnect_obj) }
    ,
    { MP_ROM_QSTR(MP_QSTR_get_connection_status), MP_ROM_PTR(&PicoWireless_get_connection_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_mac_address), MP_ROM_PTR(&PicoWireless_get_mac_address_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_ip_address), MP_ROM_PTR(&PicoWireless_get_ip_address_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_subnet_mask), MP_ROM_PTR(&PicoWireless_get_subnet_mask_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_gateway_ip), MP_ROM_PTR(&PicoWireless_get_gateway_ip_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_current_ssid), MP_ROM_PTR(&PicoWireless_get_current_ssid_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_current_bssid), MP_ROM_PTR(&PicoWireless_get_current_bssid_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_current_rssi), MP_ROM_PTR(&PicoWireless_get_current_rssi_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_current_encryption_type), MP_ROM_PTR(&PicoWireless_get_current_encryption_type_obj) },

    { MP_ROM_QSTR(MP_QSTR_start_scan_networks), MP_ROM_PTR(&PicoWireless_start_scan_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_scan_networks), MP_ROM_PTR(&PicoWireless_get_scan_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_ssid_networks), MP_ROM_PTR(&PicoWireless_get_ssid_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_enc_type_networks), MP_ROM_PTR(&PicoWireless_get_enc_type_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_bssid_networks), MP_ROM_PTR(&PicoWireless_get_bssid_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_channel_networks), MP_ROM_PTR(&PicoWireless_get_channel_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_rssi_networks), MP_ROM_PTR(&PicoWireless_get_rssi_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_req_host_by_name), MP_ROM_PTR(&PicoWireless_req_host_by_name_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_host_by_name), MP_ROM_PTR(&PicoWireless_get_host_by_name_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_fw_version), MP_ROM_PTR(&PicoWireless_get_fw_version_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_time), MP_ROM_PTR(&PicoWireless_get_time_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_power_mode), MP_ROM_PTR(&PicoWireless_set_power_mode_obj) },

    { MP_ROM_QSTR(MP_QSTR_wifi_set_ap_network), MP_ROM_PTR(&PicoWireless_wifi_set_ap_network_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_ap_passphrase), MP_ROM_PTR(&PicoWireless_wifi_set_ap_passphrase_obj) },
    { MP_ROM_QSTR(MP_QSTR_ping), MP_ROM_PTR(&PicoWireless_ping_obj) },

    { MP_ROM_QSTR(MP_QSTR_debug), MP_ROM_PTR(&PicoWireless_debug_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_temperature), MP_ROM_PTR(&PicoWireless_get_temperature_obj) },
    { MP_ROM_QSTR(MP_QSTR_pin_mode), MP_ROM_PTR(&PicoWireless_pin_mode_obj) },

    { MP_ROM_QSTR(MP_QSTR_digital_write), MP_ROM_PTR(&PicoWireless_digital_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_analog_write), MP_ROM_PTR(&PicoWireless_analog_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_digital_read), MP_ROM_PTR(&PicoWireless_digital_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_analog_read), MP_ROM_PTR(&PicoWireless_analog_read_obj) },

    { MP_ROM_QSTR(MP_QSTR_server_start), MP_ROM_PTR(&PicoWireless_server_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_client_start), MP_ROM_PTR(&PicoWireless_client_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_client_stop), MP_ROM_PTR(&PicoWireless_client_stop_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_server_state), MP_ROM_PTR(&PicoWireless_get_server_state_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_client_state), MP_ROM_PTR(&PicoWireless_get_client_state_obj) },
    { MP_ROM_QSTR(MP_QSTR_avail_data), MP_ROM_PTR(&PicoWireless_avail_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_avail_server), MP_ROM_PTR(&PicoWireless_avail_server_obj) },

    { MP_ROM_QSTR(MP_QSTR_get_data), MP_ROM_PTR(&PicoWireless_get_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_data_buf), MP_ROM_PTR(&PicoWireless_get_data_buf_obj) },
    { MP_ROM_QSTR(MP_QSTR_insert_data_buf), MP_ROM_PTR(&PicoWireless_insert_data_buf_obj) },
    { MP_ROM_QSTR(MP_QSTR_send_udp_data), MP_ROM_PTR(&PicoWireless_send_udp_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_send_data), MP_ROM_PTR(&PicoWireless_send_data_obj) },
    { MP_ROM_QSTR(MP_QSTR_check_data_sent), MP_ROM_PTR(&PicoWireless_check_data_sent_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_socket), MP_ROM_PTR(&PicoWireless_get_socket_obj) },

    { MP_ROM_QSTR(MP_QSTR_wifi_set_ent_identity), MP_ROM_PTR(&PicoWireless_wifi_set_ent_identity_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_ent_username), MP_ROM_PTR(&PicoWireless_wifi_set_ent_username_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_ent_password), MP_ROM_PTR(&PicoWireless_wifi_set_ent_password_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_set_ent_enable), MP_ROM_PTR(&PicoWireless_wifi_set_ent_enable_obj) },
};
STATIC MP_DEFINE_CONST_DICT(PicoWireless_locals_dict, PicoWireless_locals_dict_table);

/***** Class Definition *****/
// NOTE: The type definition has been updated, acording to the new guidelines "https://github.com/micropython/micropython/wiki/Build-Troubleshooting"
MP_DEFINE_CONST_OBJ_TYPE(
    PicoWireless_type,
    MP_QSTR_picowireless,
    MP_TYPE_FLAG_NONE,
    //print, PicoWireless_print,
    make_new, PicoWireless_make_new,
    locals_dict, &PicoWireless_locals_dict
);

/***** Globals Table *****/
STATIC const mp_map_elem_t picowireless_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_picowireless) },
    { MP_ROM_QSTR(MP_QSTR_PicoWireless), (mp_obj_t)&PicoWireless_type },
};
STATIC MP_DEFINE_CONST_DICT(mp_module_picowireless_globals, picowireless_globals_table);

/***** Module Definition *****/
const mp_obj_module_t picowireless_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_picowireless_globals,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_picowireless, picowireless_user_cmodule, MODULE_PICO_WIRELESS_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_picowireless, picowireless_user_cmodule);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////