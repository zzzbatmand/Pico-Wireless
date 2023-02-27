from esp_socket import connection_status
import piwrequests as requests
import picowireless
import time

WIFI_SSID = "MyWifif"
WIFI_PASS = "Password"

# Init PicoWireless with default pins.
wifi = picowireless.PicoWireless()
# Connect to wifi
wifi.wifi_set_passphrase(WIFI_SSID, WIFI_PASS)

# Wait for connection
while True:
    if wifi.get_connection_status() == connection_status.WL_CONNECTED:
        break
print("Connected!")

# Make http(s) request
req=requests.get(wifi, "https://jsonplaceholder.typicode.com/users")
print(req.json())
req.close()