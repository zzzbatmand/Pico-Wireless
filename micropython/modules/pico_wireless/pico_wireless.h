// Include MicroPython API.
#include "py/runtime.h"
#include "py/objstr.h"

// Type
extern const mp_obj_type_t PicoWireless_type;

// Class methods
extern mp_obj_t PicoWireless_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
//extern void PicoWireless_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);

// Declare the functions we'll make available in Python
extern mp_obj_t PicoWireless_get_network_data(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_remote_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_wifi_set_network(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_passphrase(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_key(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_config(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_set_dns(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_set_hostname(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_disconnect(mp_obj_t self_in);

extern mp_obj_t PicoWireless_get_connection_status(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_mac_address(mp_obj_t self_in);

extern mp_obj_t PicoWireless_get_ip_address(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_subnet_mask(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_gateway_ip(mp_obj_t self_in);

extern mp_obj_t PicoWireless_get_current_ssid(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_current_bssid(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_current_rssi(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_current_encryption_type(mp_obj_t self_in);

extern mp_obj_t PicoWireless_start_scan_networks(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_scan_networks(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_ssid_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_get_enc_type_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_bssid_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_channel_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_rssi_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_req_host_by_name(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_host_by_name(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_get_fw_version(mp_obj_t self_in);
extern mp_obj_t PicoWireless_get_time(mp_obj_t self_in);
extern mp_obj_t PicoWireless_set_power_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_wifi_set_ap_network(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_ap_passphrase(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_ping(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_debug(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_temperature(mp_obj_t self_in);
extern mp_obj_t PicoWireless_pin_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_digital_write(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_analog_write(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_digital_read(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_analog_read(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_server_start(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_client_start(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_client_stop(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_get_server_state(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_client_state(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_avail_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_avail_server(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_get_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_data_buf(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_insert_data_buf(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_send_udp_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

extern mp_obj_t PicoWireless_send_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_check_data_sent(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_get_socket(mp_obj_t self_in);

extern mp_obj_t PicoWireless_wifi_set_ent_identity(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_ent_username(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_ent_password(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t PicoWireless_wifi_set_ent_enable(mp_obj_t self_in);
