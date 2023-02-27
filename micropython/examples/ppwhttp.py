"""Pimoroni Pico Wireless HTTP

A super-simple HTTP server library for Pico Wireless.
"""
import time
import picowireless

from micropython import const

try:
    from secrets import WIFI_SSID, WIFI_PASS
except ImportError:
    WIFI_SSID = None
    WIFI_PASS = None


TCP_CLOSED = const(0)
TCP_LISTEN = const(1)

TCP_MODE = const(0)
UDP_MODE = const(1)
TLS_MODE = const(2)
#UDP_MULTICAST_MODE = const(3)
#TLS_BEARSSL_MODE = const(4)

TCP_STATE_CLOSED = const(0)
TCP_STATE_LISTEN = const(1)
TCP_STATE_SYN_SENT = const(2)
TCP_STATE_SVN_RCVD = const(3)
TCP_STATE_ESTABLISHED = const(4)
TCP_STATE_FIN_WAIT_1 = const(5)
TCP_STATE_FIN_WAIT_2 = const(6)
TCP_STATE_CLOSE_WAIT = const(7)
TCP_STATE_CLOSING = const(8)
TCP_STATE_LAST_ACK = const(9)
TCP_STATE_TIME_WAIT = const(10)

CLOUDFLARE_DNS = (1, 1, 1, 1)
GOOGLE_DNS = (8, 8, 8, 8)

DEFAULT_HTTP_PORT = const(80)


http_body="""GET {} HTTP/1.1
Host: {}
Connection: close

"""


class PPWHTTP():
    def __init__(self) -> None:
        self.picowireless = picowireless.PicoWireless()
        self.routes = {}
        self.sockets = []
        self.hosts = {}


    def get_socket(self, force_new=False):
        if force_new or len(self.sockets) == 0:
            socket = self.picowireless.get_socket()
            self.sockets.append(socket)
            return socket
        return self.sockets[0]


    def get_ip_address(self):
        return self.picowireless.get_ip_address()


    def set_dns(self, dns):
        self.picowireless.set_dns(dns)


    def get_host_by_name(self, hostname, no_cache=False):
        # Already an IP
        if type(hostname) is tuple and len(hostname) == 4:
            return hostname

        # Get from cached hosts
        if hostname in self.hosts and not no_cache:
            return self.hosts[hostname]

        ip = self.picowireless.get_host_by_name(hostname)
        self.hosts[hostname] = ip
        return ip


    def start_wifi(self, wifi_ssid=WIFI_SSID, wifi_pass=WIFI_PASS):
        if wifi_ssid is None or wifi_pass is None:
            raise RuntimeError("WiFi SSID/PASS required. Set them in secrets.py and copy it to your Pico, or pass them as arguments.")

        print("Connecting to {}...".format(wifi_ssid))
        self.picowireless.wifi_set_passphrase(wifi_ssid, wifi_pass)

        while True:
            if self.picowireless.get_connection_status() == 3:
                break
        print("Connected!")


    def start_server(self, http_port=DEFAULT_HTTP_PORT, timeout=5000):
        my_ip = self.picowireless.get_ip_address()
        print("Starting server...")
        server_sock = self.picowireless.get_socket()
        self.picowireless.server_start(http_port, server_sock, 0)

        t_start = time.ticks_ms()

        while time.ticks_ms() - t_start < timeout:
            state = self.picowireless.get_server_state(server_sock)
            if state == TCP_LISTEN:
                print("Server listening on {1}.{2}.{3}.{4}:{0}".format(http_port, *my_ip))
                return server_sock

        return None


    def connect_to_server(self, host_address, port, client_sock, timeout=5000, connection_mode=TCP_MODE):
        if connection_mode == TLS_MODE:
            print("Connecting to {1}:{0}...".format(port, host_address))
            self.picowireless.client_start((0, 0, 0, 0), port, client_sock, connection_mode, host_address)
        else:
            host_address = self.get_host_by_name(host_address)
            print("Connecting to {1}.{2}.{3}.{4}:{0}...".format(port, *host_address))
            self.picowireless.client_start(host_address, port, client_sock, connection_mode)

        t_start = time.ticks_ms()

        while time.ticks_ms() - t_start < timeout:
            state = self.picowireless.get_client_state(client_sock)
            if state == TCP_STATE_ESTABLISHED:
                print("Connected!")
                return True
            if state == TCP_STATE_CLOSED:
                print("Connection failed!")
                return False
            print(state)
            time.sleep(0.5)

        print("Connection timeout!")
        return False


    def http_request(self, host_address, port, request_host, request_path, handler, timeout=5000, client_sock=None, connection_mode=TCP_MODE):
        if client_sock is None:
            client_sock = self.get_socket()

        if not self.connect_to_server(host_address, port, client_sock, connection_mode=connection_mode): 
            return False

        http_request = http_body.format(request_path, request_host).replace("\n", "\r\n")

        self.picowireless.send_data(client_sock, http_request)

        t_start = time.ticks_ms()

        while True:
            if time.ticks_ms() - t_start > timeout:
                self.picowireless.client_stop(client_sock)
                print("HTTP request to {}:{} timed out...".format(host_address, port))
                return False

            avail_length = self.picowireless.avail_data(client_sock)
            if avail_length > 0:
                break

        print("Got response: {} bytes".format(avail_length))

        response = b""

        while len(response) < avail_length:
            data = self.picowireless.get_data_buf(client_sock)
            response += data

        head, body = response.split(b"\r\n\r\n", 1)
        head = head.decode("utf-8")
        dhead = {}

        for line in head.split("\r\n")[1:]:
            key, value = line.split(": ", 1)
            dhead[key] = value

        encoding = "iso-8869-1"
        content_type = "application/octet-stream"

        if "Content-Type" in dhead:
            ctype = dhead["Content-Type"].split("; ")
            content_type = ctype[0].lower()
            for c in ctype:
                if c.startswith("encoding="):
                    encoding = c[9:]

        # Handle JSON content type, this is prefixed with a length
        # which we'll parse and use to truncate the body
        if content_type == "application/json":
            if not body.startswith(b"{"):
                length, body = body.split(b"\r\n", 1)
                length = int(length, 16)
                body = body[:length]

        body = body.decode(encoding)

        handler(dhead, body)

        self.picowireless.client_stop(client_sock)


    def find_route(self, route, url, method, data):
        if len(url) > 0:
            for key, value in route.items():
                if key == url[0]:
                    return self.find_route(route[url[0]], url[1:], method, data)

                elif key.startswith("<") and key.endswith(">"):
                    key = key[1:-1]
                    if ":" in key:
                        dtype, key = key.split(":")
                    else:
                        dtype = "str"

                    if dtype == "int":
                        try:
                            data[key] = int(url[0])
                        except ValueError:
                            continue

                    else:
                        data[key] = url[0]

                    return self.find_route(value, url[1:], method, data)

            return None, None

        if method in route:
            return route[method], data

        return None, None


    def handle_http_request(self, server_sock, timeout=5000):
        t_start = time.ticks_ms()

        client_sock = self.picowireless.avail_server(server_sock)
        if client_sock in [server_sock, 255, -1]:
            return False

        print("Client connected!")

        avail_length = self.picowireless.avail_data(client_sock)
        if avail_length == 0:
            self.picowireless.client_stop(client_sock)
            return False

        request = b""

        while len(request) < avail_length:
            data = self.picowireless.get_data_buf(client_sock)
            request += data
            if time.ticks_ms() - t_start > timeout:
                print("Client timed out getting data!")
                self.picowireless.client_stop(client_sock)
                return False

        request = request.decode("utf-8")

        if len(request) > 0:
            head, body = request.split("\r\n\r\n", 1)
            dhead = {}

            for line in head.split("\r\n")[1:]:
                key, value = line.split(": ", 1)
                dhead[key] = value

        method, url, _ = head.split("\r\n", 1)[0].split(" ")

        print("Serving {} on {}...".format(method, url))

        response = None

        data = {}

        if url.startswith("/"):
            url = url[1:]
        url = url.split("/")
        handler, data = self.find_route(self.routes, url, method, data)

        # Dispatch the request to the relevant route
        if callable(handler):
            if method == "POST":
                for var in body.split("&"):
                    key, value = var.split("=")
                    data[key] = value

            if data == {}:
                response = handler(method, url)
            else:
                response = handler(method, url, data)

        if response is not None:
            response = "HTTP/1.1 200 OK\r\nContent-Length: {}\r\nContent-Type: text/html\r\n\r\n".format(len(response)) + response
            self.picowireless.send_data(client_sock, response)
            self.picowireless.client_stop(client_sock)
            print("Success! Sending 200 OK")
            return True
        else:
            self.picowireless.send_data(client_sock, "HTTP/1.1 501 Not Implemented\r\nContent-Length: 19\r\n\r\n501 Not Implemented")
            self.picowireless.client_stop(client_sock)
            print("Unhandled Request! Sending 501 OK")
            return False


    def route(self, url, methods="GET"):
        if type(methods) is str:
            methods = [methods]

        if url.startswith("/"):
            url = url[1:]

        url = url.split("/")

        def decorate(handler):
            route = self.routes
            for part in url:
                if part not in route:
                    route[part] = {}

                route = route[part]

            for method in methods:
                route[method] = handler

        return decorate
