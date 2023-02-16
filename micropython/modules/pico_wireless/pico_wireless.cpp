#include "hardware/spi.h"
#include "hardware/sync.h"
#include "pico/binary_info.h"

#include "libraries/pico_wireless/pico_wireless.hpp"

using namespace pimoroni;

#ifndef MP_OBJ_TO_PTR2
#define MP_OBJ_TO_PTR2(o, t) ((t *)(uintptr_t)(o))
#endif
#define m_new_class(cls, ...) new(m_new(cls, 1)) cls(__VA_ARGS__)
#define m_del_class(cls, ptr) ptr->~cls();m_del(cls, ptr, 1)

// Printing
// String: mp_print_str(MP_PYTHON_PRINTER, "\n"); // Just add string to end thingy.
// Variable: mp_obj_print_helper(MP_PYTHON_PRINTER, mp_obj_new_int(__), PRINT_REPR); // Replace __ with variable.

extern "C" {
#include "pico_wireless.h"

#define NOT_INITIALISED_MSG     "Cannot call this function, as picowireless is not initialised. Call picowireless.init() first."

static void mp_obj_to_string(const mp_obj_t &obj, std::string &string_out) {
    if(mp_obj_is_str_or_bytes(obj)) {
        GET_STR_DATA_LEN(obj, str, str_len);
        string_out = std::string((const char*)str);
    }
    else if(mp_obj_is_float(obj))
        mp_raise_TypeError("can't convert 'float' object to str implicitly");
    else if(mp_obj_is_int(obj))
        mp_raise_TypeError("can't convert 'int' object to str implicitly");
    else if(mp_obj_is_bool(obj))
        mp_raise_TypeError("can't convert 'bool' object to str implicitly");
    else
        mp_raise_TypeError("can't convert object to str implicitly");
}

uint32_t mp_obj_to_ip(mp_obj_t obj) {
    uint len;
    mp_obj_t *items;
    mp_obj_tuple_get(obj, &len, &items);
    uint a = mp_obj_get_int(items[0]) & 0xff;
    uint b = mp_obj_get_int(items[1]) & 0xff;
    uint c = mp_obj_get_int(items[2]) & 0xff;
    uint d = mp_obj_get_int(items[3]) & 0xff;
    return (d << 24) | (c << 16) | (b << 8) | a;
}

mp_obj_t mp_ip_to_obj(IPAddress ip) {
    mp_obj_t tuple[4];
    tuple[0] = mp_obj_new_int(ip[0]);
    tuple[1] = mp_obj_new_int(ip[1]);
    tuple[2] = mp_obj_new_int(ip[2]);
    tuple[3] = mp_obj_new_int(ip[3]);
    return mp_obj_new_tuple(4, tuple);
}

typedef struct _picowireless_obj_t {
    mp_obj_base_t base;
    PicoWireless *wireless;
} picowireless_obj_t;

// void PicoWireless_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//     (void)kind;
//     //picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);
//     mp_print_str(print, "PicoWireless(");
//     mp_print_str(print, "'<todo>'");
//     //mp_obj_print_helper(print, mp_obj_new_int(self->a), PRINT_REPR);
//     //mp_print_str(print, ", ");
//     //mp_obj_print_helper(print, mp_obj_new_int(self->b), PRINT_REPR);
//     mp_print_str(print, ")");
// }

mp_obj_t PicoWireless_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    picowireless_obj_t *self = m_new_obj(picowireless_obj_t);
    self->base.type = &PicoWireless_type;

    self->wireless = m_new_class(PicoWireless);
    if(n_args == 0) {
        self->wireless->init();
    }
    else {
        enum { ARG_cs, ARG_sck, ARG_mosi, ARG_miso, ARG_resetn, ARG_gpio0, ARG_ack };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_cs, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_sck, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_mosi, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_miso, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_resetn, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_gpio0, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_ack, MP_ARG_REQUIRED | MP_ARG_INT },
        };
        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        uint8_t cs = args[ARG_cs].u_int;
        uint8_t sck = args[ARG_sck].u_int;
        uint8_t mosi = args[ARG_mosi].u_int;
        uint8_t miso = args[ARG_miso].u_int;
        uint8_t resetn = args[ARG_resetn].u_int;
        uint8_t gpio0 = args[ARG_gpio0].u_int;
        uint8_t ack = args[ARG_ack].u_int;

        self->wireless->init(cs, sck, mosi, miso, resetn, gpio0, ack);
    }

    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t PicoWireless_reset(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_bool(self->wireless->reset());
}

mp_obj_t PicoWireless_get_network_data(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    uint8_t *ip = nullptr;
    uint8_t *mask = nullptr;
    uint8_t *gwip = nullptr;
    self->wireless->get_network_data(ip, mask, gwip);

    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_bytes(ip, WL_IPV4_LENGTH);
    tuple[1] = mp_obj_new_bytes(mask, WL_IPV4_LENGTH);
    tuple[2] = mp_obj_new_bytes(gwip, WL_IPV4_LENGTH);
    return mp_obj_new_tuple(3, tuple);
}

mp_obj_t PicoWireless_get_remote_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t *ip = nullptr;
    uint8_t *port = nullptr;
    self->wireless->get_remote_data(args[ARG_sock].u_int, ip, port);

    mp_obj_t tuple[2];
    tuple[0] = mp_obj_new_bytes(ip, WL_IPV4_LENGTH);
    tuple[1] = mp_obj_new_int((uint16_t)port[0] << 8 | (uint16_t)port[1]); //TODO verify size and ordering of port
    return mp_obj_new_tuple(2, tuple);
}

mp_obj_t PicoWireless_wifi_set_network(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ssid, ARG_passphrase };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string ssid;
    mp_obj_to_string(args[ARG_ssid].u_obj, ssid);
    return mp_obj_new_int(self->wireless->wifi_set_network(ssid));
}

mp_obj_t PicoWireless_wifi_set_passphrase(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ssid, ARG_passphrase };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_passphrase, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string ssid, passphrase;
    mp_obj_to_string(args[ARG_ssid].u_obj, ssid);
    mp_obj_to_string(args[ARG_passphrase].u_obj, passphrase);
    return mp_obj_new_int(self->wireless->wifi_set_passphrase(ssid, passphrase));
}

mp_obj_t PicoWireless_wifi_set_key(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ssid, ARG_key_idx, ARG_passphrase };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_key_idx, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_passphrase, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t key_idx = args[ARG_key_idx].u_int;

    std::string ssid, passphrase;
    mp_obj_to_string(args[ARG_ssid].u_obj, ssid);
    mp_obj_to_string(args[ARG_passphrase].u_obj, passphrase);
    return mp_obj_new_int(self->wireless->wifi_set_key(ssid, key_idx, passphrase));
}

mp_obj_t PicoWireless_config(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_valid_params, ARG_local_ip, ARG_gateway, ARG_subnet };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_valid_params, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_local_ip, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_gateway, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_subnet, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t valid_params = args[ARG_valid_params].u_int;
    uint32_t local_ip = mp_obj_to_ip(args[ARG_local_ip].u_obj);
    uint32_t gateway = mp_obj_to_ip(args[ARG_gateway].u_obj);
    uint32_t subnet = mp_obj_to_ip(args[ARG_subnet].u_obj);
    self->wireless->config(valid_params, local_ip, gateway, subnet);

    return mp_const_none;
}

mp_obj_t PicoWireless_set_dns(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_dns_server1, ARG_dns_server2 };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_dns_server1, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_dns_server2 | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t valid_params = n_args;
    uint32_t dns_server1 = mp_obj_to_ip(args[ARG_dns_server1].u_obj);
    uint32_t dns_server2 = n_args == 1 ? 0 : mp_obj_to_ip(args[ARG_dns_server2].u_obj);
    self->wireless->set_dns(valid_params, dns_server1, dns_server2);

    return mp_const_none;
}

mp_obj_t PicoWireless_set_hostname(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_hostname };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_hostname, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string hostname;
    mp_obj_to_string(args[ARG_hostname].u_obj, hostname);
    self->wireless->set_hostname(hostname);

    return mp_const_none;
}

mp_obj_t PicoWireless_disconnect(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);
    
    return mp_obj_new_int(self->wireless->disconnect());
}

mp_obj_t PicoWireless_get_connection_status(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);
    
    return mp_obj_new_int(self->wireless->get_connection_status());
}

mp_obj_t PicoWireless_get_mac_address(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);
    
    uint8_t* mac = self->wireless->get_mac_address();
    return mp_obj_new_bytes(mac, WL_MAC_ADDR_LENGTH);
}

mp_obj_t PicoWireless_get_ip_address(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    IPAddress ip;
    self->wireless->get_ip_address(ip);
    return mp_ip_to_obj(ip);
}

mp_obj_t PicoWireless_get_subnet_mask(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    IPAddress mask;
    self->wireless->get_subnet_mask(mask);
    return mp_ip_to_obj(mask);
}

mp_obj_t PicoWireless_get_gateway_ip(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    IPAddress mask;
    self->wireless->get_gateway_ip(mask);
    return mp_ip_to_obj(mask);
}

mp_obj_t PicoWireless_get_current_ssid(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    std::string ssid = self->wireless->get_current_ssid();
    return mp_obj_new_str(ssid.c_str(), ssid.length());
}

mp_obj_t PicoWireless_get_current_bssid(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    uint8_t* bssid = self->wireless->get_current_bssid();
    return mp_obj_new_bytes(bssid, WL_MAC_ADDR_LENGTH);
}

mp_obj_t PicoWireless_get_current_rssi(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->get_current_rssi());
}

mp_obj_t PicoWireless_get_current_encryption_type(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->get_current_encryption_type());
}

// TODO: Why is this even here?
mp_obj_t PicoWireless_start_scan_networks(mp_obj_t self_in) {
    // This doesn't actually *do* anything, so might as well save a few instructions!
    
    return mp_const_true;
}

mp_obj_t PicoWireless_get_scan_networks(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->get_scan_networks());
}

mp_obj_t PicoWireless_get_ssid_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_network_item };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_network_item, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t network_item = args[ARG_network_item].u_int;
    const char* ssid = self->wireless->get_ssid_networks(network_item);
    if(ssid != nullptr) {
        return mp_obj_new_str(ssid, strnlen(ssid, WL_SSID_MAX_LENGTH));
    }
    else
        return mp_const_none;
}

mp_obj_t PicoWireless_get_enc_type_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_network_item };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_network_item, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t network_item = args[ARG_network_item].u_int;
    return mp_obj_new_int(self->wireless->get_enc_type_networks(network_item));
}

mp_obj_t PicoWireless_get_bssid_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_network_item };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_network_item, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t network_item = args[ARG_network_item].u_int;
    uint8_t* bssid = nullptr;
    self->wireless->get_bssid_networks(network_item, bssid);
    if(bssid != nullptr) {
        return mp_obj_new_bytes(bssid, WL_MAC_ADDR_LENGTH);
    }
    else
        return mp_const_none;
}

mp_obj_t PicoWireless_get_channel_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_network_item };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_network_item, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t network_item = args[ARG_network_item].u_int;
    return mp_obj_new_int(self->wireless->get_channel_networks(network_item));
}

mp_obj_t PicoWireless_get_rssi_networks(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_network_item };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_network_item, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t network_item = args[ARG_network_item].u_int;
    return mp_obj_new_int(self->wireless->get_rssi_networks(network_item));
}

mp_obj_t PicoWireless_req_host_by_name(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_hostname };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_hostname, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string hostname;
    mp_obj_to_string(args[ARG_hostname].u_obj, hostname);

    return mp_obj_new_bool(self->wireless->req_host_by_name(hostname));
}

mp_obj_t PicoWireless_get_host_by_name(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    if(n_args == 1) {
        enum { ARG_self };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        IPAddress ip;
        if(self->wireless->get_host_by_name(ip)) {
            return mp_ip_to_obj(ip);
        }
    }
    else {
        enum { ARG_self, ARG_hostname };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_hostname, MP_ARG_REQUIRED | MP_ARG_OBJ },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        std::string hostname;
        mp_obj_to_string(args[ARG_hostname].u_obj, hostname);

        IPAddress ip;
        if(self->wireless->get_host_by_name(hostname, ip)) {
            return mp_ip_to_obj(ip);
        }
    }
    
    return mp_const_none;
}

mp_obj_t PicoWireless_get_fw_version(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    const char* fw_ver = self->wireless->get_fw_version();
    return mp_obj_new_str(fw_ver, strnlen(fw_ver, WL_FW_VER_LENGTH));
}

mp_obj_t PicoWireless_get_time(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->get_time());
}

mp_obj_t PicoWireless_set_power_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t mode = args[ARG_mode].u_int;
    self->wireless->set_power_mode(mode);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_wifi_set_ap_network(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ssid, ARG_channel };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_channel, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string ssid;
    mp_obj_to_string(args[ARG_ssid].u_obj, ssid);

    uint8_t channel = args[ARG_channel].u_int;        
    return mp_obj_new_int(self->wireless->wifi_set_ap_network(ssid, channel));
}

mp_obj_t PicoWireless_wifi_set_ap_passphrase(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ssid, ARG_passphrase, ARG_channel };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_passphrase, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_channel, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string ssid, passphrase;
    mp_obj_to_string(args[ARG_ssid].u_obj, ssid);
    mp_obj_to_string(args[ARG_passphrase].u_obj, passphrase);

    uint8_t channel = args[ARG_channel].u_int;        
    return mp_obj_new_int(self->wireless->wifi_set_ap_passphrase(ssid, passphrase, channel));
}

mp_obj_t PicoWireless_ping(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_ip_address, ARG_count };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_ip_address, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_count, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint32_t ip_address = mp_obj_to_ip(args[ARG_ip_address].u_obj);
    uint8_t count = args[ARG_count].u_int;
    return mp_obj_new_int(self->wireless->ping(ip_address, count));
}

mp_obj_t PicoWireless_debug(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_on };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_on, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t on = args[ARG_on].u_int;
    self->wireless->debug(on);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_get_temperature(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_float(self->wireless->get_temperature());
}

mp_obj_t PicoWireless_pin_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t pin = args[ARG_pin].u_int;
    uint8_t mode = args[ARG_mode].u_int;
    self->wireless->pin_mode(pin, mode);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_digital_write(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_value };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_value, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t pin = args[ARG_pin].u_int;
    uint8_t value = args[ARG_value].u_int;
    self->wireless->digital_write(pin, value);

    return mp_const_none;
}

mp_obj_t PicoWireless_analog_write(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_value };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_value, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t pin = args[ARG_pin].u_int;
    uint8_t value = args[ARG_value].u_int;
    self->wireless->analog_write(pin, value);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_digital_read(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_value };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t pin = args[ARG_pin].u_int;
    return mp_obj_new_bool(self->wireless->digital_read(pin));
}

mp_obj_t PicoWireless_analog_read(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    if(n_args == 2) {
        enum { ARG_self, ARG_pin };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        uint8_t pin = args[ARG_pin].u_int;
        return mp_obj_new_int(self->wireless->analog_read(pin) * 16); //Returns a 16 bit-ish* number as per CircuitPython
    }
    else {
        enum { ARG_self, ARG_pin, ARG_atten };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_esp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_atten, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        uint8_t pin = args[ARG_pin].u_int;
        uint8_t atten = args[ARG_atten].u_int;
        return mp_obj_new_int(self->wireless->analog_read(pin, atten) * 16); //Returns a 16 bit-ish* number as per CircuitPython
    }
    //NOTE *better way would be to mult by 65535 then div by 4095
    
    return mp_const_none;
}

mp_obj_t PicoWireless_server_start(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // I have removed IP as it was only used in UDP multicast.
    enum { ARG_self, ARG_port, ARG_sock, ARG_protocol_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_prt, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_protocol_mode, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint16_t port = args[ARG_port].u_int;
    uint8_t sock = args[ARG_sock].u_int;
    uint8_t protocol_mode = args[ARG_protocol_mode].u_int;
    self->wireless->start_server(port, sock, protocol_mode);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_client_start(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    if(n_args == 5) {
        enum { ARG_self, ARG_ip_address, ARG_port, ARG_sock, ARG_protocol_mode };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_ip_address, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_prt, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_protocol_mode, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        uint32_t ip_address = mp_obj_to_ip(args[ARG_ip_address].u_obj);
        uint16_t port = args[ARG_port].u_int;
        uint8_t sock = args[ARG_sock].u_int;
        uint8_t protocol_mode = args[ARG_protocol_mode].u_int;
        self->wireless->start_client(ip_address, port, sock, protocol_mode);
    }
    else {
        enum { ARG_self, ARG_ip_address, ARG_port, ARG_sock, ARG_protocol_mode, ARG_hostname };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_ip_address, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_prt, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_protocol_mode, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_hostname, MP_ARG_REQUIRED | MP_ARG_OBJ },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        std::string hostname;
        mp_obj_to_string(args[ARG_hostname].u_obj, hostname);

        uint32_t ip_address = mp_obj_to_ip(args[ARG_ip_address].u_obj);
        uint16_t port = args[ARG_port].u_int;
        uint8_t sock = args[ARG_sock].u_int;
        uint8_t protocol_mode = args[ARG_protocol_mode].u_int;
        self->wireless->start_client(hostname, ip_address, port, sock, protocol_mode);
    }
    
    return mp_const_none;
}

mp_obj_t PicoWireless_client_stop(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    self->wireless->stop_client(sock);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_get_server_state(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    return mp_obj_new_int(self->wireless->get_server_state(sock));
}

mp_obj_t PicoWireless_get_client_state(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    return mp_obj_new_int(self->wireless->get_client_state(sock));
}

mp_obj_t PicoWireless_avail_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    return mp_obj_new_int(self->wireless->avail_data(sock));
}

mp_obj_t PicoWireless_avail_server(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    return mp_obj_new_int(self->wireless->avail_server(sock));
}

mp_obj_t PicoWireless_get_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock, ARG_peek };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_peek, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t *data = nullptr;
    if(self->wireless->get_data(args[ARG_sock].u_int, data, args[ARG_peek].u_int)) {
        return mp_obj_new_int(*data);
    }
    
    return mp_const_none;
}

mp_obj_t PicoWireless_get_data_buf(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    picowireless_obj_t *self = nullptr;
    uint16_t data_len = 512;
    uint8_t sock;

    if(n_args == 2) {
        enum { ARG_self, ARG_sock };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        sock = args[ARG_sock].u_int;
    }
    else {
        enum { ARG_self, ARG_sock, ARG_length };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
            { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_length, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

        data_len = args[ARG_length].u_int;
        sock = args[ARG_sock].u_int;
    }

    uint8_t *data = (uint8_t *)malloc(data_len);
    if(self->wireless->get_data_buf(sock, data, &data_len)) {
        mp_obj_t response = mp_obj_new_bytes(data, data_len);
        free(data);
        return response;
    }

    free(data);
    return mp_const_none;
}

mp_obj_t PicoWireless_insert_data_buf(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock, ARG_data};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_data, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_data].u_obj, &bufinfo, MP_BUFFER_READ);

    return mp_obj_new_bool(self->wireless->insert_data_buf(args[ARG_sock].u_int, (uint8_t *)bufinfo.buf, bufinfo.len));
}

mp_obj_t PicoWireless_send_udp_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    uint8_t sock = args[ARG_sock].u_int;
    return mp_obj_new_bool(self->wireless->send_udp_data(sock));
}

mp_obj_t PicoWireless_send_data(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock, ARG_data};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_data, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_data].u_obj, &bufinfo, MP_BUFFER_READ);

    return mp_obj_new_int(self->wireless->send_data(args[ARG_sock].u_int, (uint8_t *)bufinfo.buf, bufinfo.len));
}

mp_obj_t PicoWireless_check_data_sent(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sock };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sock, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->check_data_sent(args[ARG_sock].u_int));
}

mp_obj_t PicoWireless_get_socket(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    return mp_obj_new_int(self->wireless->get_socket());
}

mp_obj_t PicoWireless_wifi_set_ent_identity(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_identity };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_identity, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string identity;
    mp_obj_to_string(args[ARG_identity].u_obj, identity);
    self->wireless->wifi_set_ent_identity(identity);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_wifi_set_ent_username(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_username };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_username, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string username;
    mp_obj_to_string(args[ARG_username].u_obj, username);
    self->wireless->wifi_set_ent_username(username);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_wifi_set_ent_password(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_password };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_self, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_password, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    picowireless_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, picowireless_obj_t);

    std::string password;
    mp_obj_to_string(args[ARG_password].u_obj, password);
    self->wireless->wifi_set_ent_password(password);
    
    return mp_const_none;
}

mp_obj_t PicoWireless_wifi_set_ent_enable(mp_obj_t self_in) {
    picowireless_obj_t *self = MP_OBJ_TO_PTR2(self_in, picowireless_obj_t);

    self->wireless->wifi_set_ent_enable();
    
    return mp_const_none;
}
}
