import picowireless

# Pins are defined as follows (cs, sck, mosi, miso, reset/_EN, gpio0, ack/_Ready)
# The default values can be found in the guide
wireless = picowireless.PicoWireless(7, 18, 19, 16, 11, 2, 10)
# Alternative, if no pins are use, then it uses the default ones defined.
#wireless = picowireless.PicoWireless()

# Not realy neccesarry, as it does nothing.
wireless.start_scan_networks()

networks = wireless.get_scan_networks()
print("Found {} network(s)...".format(networks))
for network in range(networks):
    ssid = wireless.get_ssid_networks(network)
    print("{}: {}".format(network, ssid))
print("\n")

