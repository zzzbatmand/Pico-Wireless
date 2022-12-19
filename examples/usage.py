import time
import picowireless as aa
from micropython import const
import urequests_custom as ppp

WIFI_SSID = "reatuirytuy" #"Mit_wifi"
WIFI_PASS = "lagkageidag" #"12345678"

picowireless = aa.PicoWireless()
picowireless.wifi_set_passphrase(WIFI_SSID, WIFI_PASS)

while True:
    if picowireless.get_connection_status() == 3:
        break
print("Connected!")

obj={
    "system": "trailerlog-display",
    "action": "get-temperatures",
    "token": "7XUWsJp0HqEzIHE76jPv6Hhw1eNed3EWl6Sb99kRb6NJWbqf5jKOiLy82xSfKcULNQpfPCYINNsGeelxTUFJca7BE6XyaVtUxqHGNSBHr3jNHPopZDnNAAo7aj2hOjM5p7Y4yTrZLQlwJL3qwUbnAwjctXlAWbBrzViKx96bciTqsacQ4TNx43FiUJm3oadUJAF53RnRzffIa99vOOgNv85lJoHu6bfCuaqzoHD27KO9VtqnKc6VCrjDilyWGldh",
    "trailer_id": 159
}
ff=ppp.get(picowireless, "http://app.trailerlog.com/app/api.php", json=obj)
print(ff.text)
