import time
import picowireless as wireless
from micropython import const
import urequests_custom as urequests

WIFI_SSID = "MyWifif"
WIFI_PASS = "Password"

picowireless = wireless.PicoWireless()
picowireless.wifi_set_passphrase(WIFI_SSID, WIFI_PASS)

while True:
    if picowireless.get_connection_status() == 3:
        break
print("Connected!")

ff=urequests.get(picowireless, "https://reqres.in/api/users?page=2")
print(ff.json())
ff.close()