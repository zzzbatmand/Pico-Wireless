# https://forums.pimoroni.com/t/pico-wireless-pack-fetching-data-from-web/17215/15


import time
import picowireless
from micropython import const

WIFI_SSID = "MyWifif"
WIFI_PASS = "Password"

CLOUDFLARE_DNS = (1, 1, 1, 1)
GOOGLE_DNS = (8, 8, 8, 8)
DEF_DNS = GOOGLE_DNS

TCP_MODE = const(0)
HTTP_REQUEST_DELAY = const(30)
HTTP_PORT = 80
HTTP_REQUEST_HOST = "api.thingspeak.com"
HTTP_REQUEST_PATH = "/channels/1417/field/2/last.txt"

wifi = picowireless.PicoWireless()

def connect(host_address, port, client_sock, timeout=1000):
    wifi.client_start(host_address, port, client_sock, TCP_MODE)

    t_start = time.time()
    timeout /= 1000.0

    while time.time() - t_start < timeout:
        state = wifi.get_client_state(client_sock)
        if state == 4:
            return True
        time.sleep(1.0)

    return False


def http_request(client_sock, host_address, port, request_host, request_path, handler, timeout=5000):
    print("Connecting to {1}.{2}.{3}.{4}:{0}...".format(port, *host_address))
    if not connect(host_address, port, client_sock):
        print("Connection failed!")
        return False
    print("Connected!")

    http_request = """GET {} HTTP/1.1
Host: {}
Connection: close

""".format(request_path, request_host).replace("\n", "\r\n")

    wifi.send_data(client_sock, http_request)

    t_start = time.time()

    while True:
        if time.time() - t_start > timeout:
            wifi.client_stop(client_sock)
            print("HTTP request to {}:{} timed out...".format(host_address, port))
            return False

        avail_length = wifi.avail_data(client_sock)
        if avail_length > 0:
            break

    print("Got response: {} bytes".format(avail_length))

    response = b""

    while len(response) < avail_length:
        data = wifi.get_data_buf(client_sock)
        response += data

    response = response.decode("utf-8")

    head, body = response.split("\r\n\r\n", 1)
    dhead = {}

    for line in head.split("\r\n")[1:]:
        key, value = line.split(": ", 1)
        dhead[key] = value

    handler(dhead, body)

    wifi.client_stop(client_sock)
    

print("Connecting to {}...".format(WIFI_SSID))
wifi.wifi_set_passphrase(WIFI_SSID, WIFI_PASS)

while True:
    if wifi.get_connection_status() == 3:
        break
print("Connected!")

# Get our own local IP!
my_ip = wifi.get_ip_address()
print("Local IP: {}.{}.{}.{}".format(*my_ip))

# Resolve and cache the IP address
wifi.set_dns(DEF_DNS)
http_address = wifi.get_host_by_name(HTTP_REQUEST_HOST)
print("Resolved {} to {}.{}.{}.{}".format(HTTP_REQUEST_HOST, *http_address))

client_sock = wifi.get_socket()


def handler(head, body):
    print(head)
    print(body)
    if head["Status"] == "200 OK":
        print("Got response {}".format(body))
    else:
        print("Error: {}".format(head["Status"]))

http_request(client_sock, http_address, HTTP_PORT, HTTP_REQUEST_HOST, HTTP_REQUEST_PATH, handler)
