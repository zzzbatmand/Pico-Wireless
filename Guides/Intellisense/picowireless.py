# class connection_status():
#     WL_NO_SHIELD        = 255,
#     WL_IDLE_STATUS      = 0,
#     WL_NO_SSID_AVAIL    = 1,
#     WL_SCAN_COMPLETED   = 2,
#     WL_CONNECTED        = 3,
#     WL_CONNECT_FAILED   = 4,
#     WL_CONNECTION_LOST  = 5,
#     WL_DISCONNECTED     = 6

# class protocols():
#     TCP_MODE    = 0,
#     UDP_MODE    = 1,
#     TLS_MODE    = 2

# TODO:
# Many items returns int when it might as well be a bool.

class PicoWireless:
    def __init__(self, cs=None, sck=None, mosi=None, miso=None, resetn=None, gpio0=None, ack=None):
        # type: (None|int, None|int, None|int, None|int, None|int, None|int, None|int) -> None
        """
        Init connection to the ESP.

        Pins are optional, but all is required, if used.

        Pins
        ----------
        - ``cs    : SPI Chip Select``
        - ``sck   : SPI Clock``
        - ``mosi  : SPI MOSI``
        - ``miso  : SPI MISO``
        - ``restn : ESP Reset pin``
        - ``gpio0 : ESP GPIO0``
        - ``ack   : ESP Acknowledge``
        """
        return

    def reset(self):
        # type: () -> bool
        """
        Reset the ESP32

        :return: Boolean weather the modem was reset correctly.
        """
        return bool()

    def get_network_data(self):
        # type: () -> tuple[bytes, bytes, bytes]
        """
        Get the IP, Subnet, and Gateway IP.
        
        :return: Tupel of (ip, subnet, gateway)
        """
        return tuple()

    def get_remote_data(self, sock):
        # type: (int) -> tuple[bytes, int]
        """
        Get the IP and port of remote server connected to a given socket.
        
        :param sock: Socket index
        
        :return: Tupel of (ip, port)
        """
        return tuple()

    def wifi_set_network(self, ssid):
        # type: (str) -> int
        """
        Connect to an open WiFi network with a given ssid.

        :param ssid: Network name
        """
        return int()

    def wifi_set_passphrase(self, ssid, passphrase):
        # type: (str, str) -> int
        """
        Connect to a WiFi network with a given ssid and password.

        :param ssid: Network name
        :param passphrase: Password
        """
        return int()

    def wifi_set_key(self, ssid, key_idx, passphrase):
        # type: (str, int, str) -> int
        """
        Connect to a WiFi network with key and stuff :/
        """
        return int()    

    def config(self, valid_params, local_ip, gateway, subnet):
        # type: (int, tuple, tuple, tuple) -> None
        """
        Set IP, Subnet, and Gateway IP.
        
        :param valid_params: NO-OP (Does nothing)
        :param local_ip: IP address to set
        :param gateway: Gateway address to set
        :param subnet: Subnet Mask to set
        """

    def set_dns(self, dns_server1, dns_server2=None):
        # type: (tuple, tuple|None) -> None
        """
        Set DNS server.
        
        :param dns_server1: DNS1
        :param dns_server2: DNS2
        """

    def set_hostname(self, hostname):
        # type: (str) -> None
        """
        Set the hostname.
        
        :param hostname: Hostname as a string
        """

    def disconnect(self):
        # type: () -> int
        """
        Disconnect wifi

        :return: 1
        """
        return int()

    def get_connection_status(self):
        # type: () -> int
        """
        Get connection status.

        Return
        ----------
        - ``WL_NO_SHIELD        = 255,``
        - ``WL_IDLE_STATUS      = 0,``
        - ``WL_NO_SSID_AVAIL    = 1,``
        - ``WL_SCAN_COMPLETED   = 2,``
        - ``WL_CONNECTED        = 3,``
        - ``WL_CONNECT_FAILED   = 4,``
        - ``WL_CONNECTION_LOST  = 5,``
        - ``WL_DISCONNECTED     = 6``
        """
        return int()

    def get_mac_address(self):
        # type () -> bytes
        """
        Get the MAC address of the ESP32
        """
        return bytes()

    def get_ip_address(self):
        # type: () -> tuple[int, int, int, int]
        """
        Get the current IP address.

        :return: IP Address
        """
        return tuple()

    def get_subnet_mask(self):
        # type () -> tuple[int, int, int, int]
        """
        Get the subnet mask of the ESP32
        """
        return tuple()

    def get_gateway_ip(self):
        # type () -> tuple[int, int, int, int]
        """
        Get the gateway IP address of the ESP32
        """
        return tuple()

    def get_current_ssid(self):
        # type: () -> str
        """
        Get the SSID of the currently connected network.

        :return: SSID as a string
        """
        return str()

    def get_current_bssid(self):
        # type: () -> bytes
        """
        Get the BSSID of the currently connected network.

        :return: BSSID as bytes
        """
        return bytes()

    def get_current_rssi(self):
        # type: () -> int
        """
        Get the RSSI of the currently connected network.

        :return: RSSI as an integer
        """
        return int()

    def get_current_encryption_type(self):
        # type: () -> int
        """
        Get the encryption type of the currently connected network.

        :return: Encryption type as an integer
        """
        return int()

    def get_scan_networks(self):
        # type: () -> int
        """
        Scan for networks and get the number of networks found.

        :return: Number of networks found
        """
        return int()

    def get_ssid_networks(self, network_item):
        # type: (int) -> str
        """
        Get the SSID of a scanned network.

        :param network_item: Index of network item, given from `get_scan_networks()`

        :return: SSID of network
        """
        return str()

    def get_enc_type_networks(self, network_item):
        # type: (int) -> int
        """
        Get the encryption type of a selected network.

        :param network_item: Index of the network item

        :return: Encryption type as an integer
        """
        return int()

    def get_bssid_networks(self, network_item):
        # type: (int) -> bytes|None
        """
        Get the BSSID of a selected network.

        :param network_item: Index of the network item

        :return: BSSID as bytes
        """
        return bytes()

    def get_channel_networks(self, network_item):
        # type: (int) -> int
        """
        Get the channel of a selected network.

        :param network_item: Index of the network item

        :return: Channel as an integer
        """
        return int()

    def get_rssi_networks(self, network_item):
        # type: (int) -> int
        """
        Get the RSSI of a selected network.

        :param network_item: Index of the network item

        :return: RSSI as an integer
        """
        return int()

    def req_host_by_name(self, hostname):
        # type: (str) -> bool
        """
        Find the IP of a given hostname.

        :param hostname: Host to find

        :return: Whether it succeeded or not
        """
        return bool()

    def get_host_by_name(self):
        # type: () -> str
        """
        Get the IP that was located using `req_host_by_name()`

        :return: IP address as a string
        """
        return str()

    def get_fw_version(self):
        # type: () -> str
        """
        Get the current NINO-FW version
        """
        return str()

    def get_time(self):
        # type: () -> int
        """
        Get unix timestam from internet.

        :return: Time in seconds since Jan. 1. 1970
        """
        return int()

    def set_power_mode(self, mode):
        # type: (int) -> None
        """
        Set the power mode.
        NO-OP, not implemented as the ESP don't have this.
        
        :param mode: Power mode
        """

    def wifi_set_ap_network(self, ssid, channel):
        # type: (str, int) -> int
        """
        Create an open network, with a given SSID and channel

        :param ssid: Name of the AP
        :param channel: Channel of the AP

        :return: Bollean value of the weather creation was successful (1 | 0)
        """
        return int()

    def wifi_set_ap_passphrase(self, ssid, passphrase, channel):
        # type: (str, str, int) -> int
        """
        Create a network, with a given SSID, password, and channel

        :param ssid: Name of the AP
        :param passphrase: Password of the AP
        :param channel: Channel of the AP

        :return: Bollean value of the weather creation was successful (1 | 0)
        """
        return int()

    def ping(self, ip_address, count):
        # type: (str, int) -> int
        """
        Ping the given ip/host n ammount of times.
        """
        return int()

    def debug(self, on):
        # type: (bool) -> None
        """
        Enables/Disables serial debug on the ESP32 Modem

        :param on: On/Off - True/False
        """

    def get_temperature(self):
        # type: () -> float
        """
        Get the temperature of the ESP32
        """
        return float()

    def pin_mode(self, esp_pin, mode):
        # type: (int, int) -> None
        """
        Set the mode of a GPIO pin on the ESP32

        :param esp_pin: Pin number of the esp
        :param mode: Mode to set the pin in (IN | OUT | ...)
        """

    def digital_write(self, esp_pin, value):
        # type: (int, int) -> None
        """
        Write a 0 or 1 to a GPIO pin on the ESP32

        :param esp_pin: Pin number of the esp
        :param value: Bollean value of the pin (1 | 0)
        """

    def analog_write(self, esp_pin, value):
        # type: (int, int) -> None
        """
        Write a 0 to 255 "analog" value to a GPIO pin on the ESP32

        :param esp_pin: Pin number of the esp
        :param value: Value of the pin (0 - 255)
        """

    def digital_read(self, esp_pin):
        # type: (int) -> bool
        """
        Read a 0 (LOW) or 1 (HIGH) from a GPIO pin on the ESP32

        :param esp_pin: Pin number of the esp

        :return: Status of the pin
        """
        return bool()

    def analog_read(self, esp_pin):
        # type: (int) -> int
        """
        Read an analog value from a GPIO pin on the ESP32

        :param esp_pin: Pin number of the esp
        
        :return: Value of the ADC (0 - 65535)
        """
        return int()

    def server_start(self, prt, sock, protocol_mode):
        # type: (int, int, int) -> None
        """
        Start a socket server TCP/UDP

        :param prt: Port to connect to.
        :param sock: Socket index
        :param protocol_mode: Int of the type protocols (``TCP`` == 0, ``UDP`` == 1)
        """
        return

    def client_start(self, ip, prt, sock, protocol_mode, hostname=''):
        # type: (tuple, int, int, int, str) -> None
        """
        Connect to a remote server.

        :param ip_address: IP address or domain of the remote server.
        :param prt: Port to connect to.
        :param sock: Socket index
        :param protocol_mode: Int of the type protocols (``TCP`` == 0, ``UDP`` == 1)
        :param hostname: (OPTIONAL) Hostname
        """
        return

    def client_stop(self, sock):
        # type: (int) -> None
        """
        Close the given socket.

        :param sock: Socket index
        """
        return

    def get_server_state(self, sock):
        # type: (int) -> int
        """
        Get boolean status of server, as int

        :param sock: Socket index

        :return: Socket online 1 or 0 (True or False)
        """
        return int()

    def get_client_state(self, sock):
        # type: (int) -> int
        """
        Gets the state of the given socket number.

        :param sock: Socket index
        
        :return: 4 == Connected
        """
        return int()

    def avail_data(self, sock):
        # type: (int) -> int
        """
        Get the ammount of available data from the given socket.

        :param sock: Socket index
        
        :return: Number of bytes
        """
        return int()

    def avail_server(self, sock):
        # type: (int) -> int
        """
        Get client socket connection, from server socet.
        Use the client socket, to get and send data.

        :param sock: Server socket index

        :return: Client socket index
        """
        return int()

    def get_data(self, sock, peek):
        # type: (int, int) -> int|None
        """
        Peek or get a single byte of data, from a given socket.

        :param sock: Socket index
        :param peek: Boolean on weather to peek or not.
        
        :return: Byte of data
        """
        return int()

    def get_data_buf(self, sock, length=512):
        # type: (int, int) -> bytes|None
        """
        Get length ammount of data from the given socket.

        :param sock: Socket index
        :param length: Ammout of data to get
        
        :return: Data bytes, or None
        """
        return bytes()

    def insert_data_buf(self, sock, data):
        # type: (int, bytes) -> bool
        """
        UDP only. Writes to the socket output buffer.

        :param sock: Socket index
        :param data: Data to insert
        
        :return: Was write successful
        """
        return bool()

    def send_udp_data(self, sock):
        # type: (int) -> bool
        """
        UDP only. Writes the socket UDP end packet.

        :param sock: Socket index
        
        :return: Was write successful
        """
        return bool()

    def send_data(self, sock, data):
        # type: (int, str|bytes) -> int
        """
        Send a data buffer over TCP (server/client) or TLS type sockets

        :param sock: Socket index
        
        :return: Number of bytes sent
        """
        return int()

    def check_data_sent(self, sock):
        # type: (int) -> int
        """
        Check if TCP data was sent.
        NO-OP, as "send_data" does simmilar.

        :param sock: Socket index
        
        :return: Was data sent
        """
        return int()

    def get_socket(self):
        # type: () -> int
        """
        Get the first available socket.

        :return: Socket index
        """
        return int()