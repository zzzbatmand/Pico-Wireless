import picowireless

# Define pico-wireless with default pins.
wireless = picowireless.PicoWireless()

# Alternative, if you want to use different pins, then call it like this.
#wireless = picowireless.PicoWireless(7, 18, 19, 16, 11, 2, 10)
# Pins are defined as follows (cs, sck, mosi, miso, reset/_EN, gpio0, ack/_Ready)

networks = wireless.get_scan_networks()
print("Found {} network(s)...".format(networks))
for network in range(networks):
    ssid = wireless.get_ssid_networks(network)
    print("{}: {}".format(network, ssid))
print("\n")

