# Pico-Wireless
Pico-Wireless is a way to connect a regular Raspberry Pi Pico / Pico-W together with an ESP32, to get wifi.

I know that the pico-w already has WiFi, but this can be used, with only the RP2040, or if multiple WiFi networks are needed..

This project is a modifyed version of the "picowireless" module from [Pimoroni-Pico](https://github.com/pimoroni/pimoroni-pico).  
I have modifyed it to allow multiple ESP32 devices, instead of just one.  

I have also made a custom version of the ESP32-NINA that is used on the ESP32.  
This version can be found [HERE](https://github.com/zzzbatmand/NINA-Arduino) and works with regular arduino, and should work with all ESP32 versions.

**TESTED:**
- ESP32
- ESP32-C3

**NOT TESTED:**
- ESP32-S2
- ESP32-S3
- ESP32-C6
- ESP8266

I don't have the other modules to test with, so they wil stay under "NOT TESTED" for the time being.

<br>

# Building custom micropython firmware (ESP32 WiFi)
You can either use the pre-build version, found in the [release]() page, and install it as shown [here](Guides/Install_and_Use.md).  

Or you can use the guide [here](Guides/Build.md), to build one yourself.

<br>

# Install firmware
The firmware is a regular `.uf2` firmware, that just can be uploaded to the pico.  
If you don't know how to do that, just google it, or check [this](Guides/Install_and_Use.md) simple guide.  

<br>

# Connect the Pico to the ESP32
Follow [this](Guides/Wiring.md) guide to connect the two modules together.  

The pins might differ from ESP32 versions, as this guide is made with the ESP32-C3 in mind, I strongly recommend you read the mentioned guide to connect it correctly!

<br>

# Examples
Examples can be found under "`micropython/Examples`"

<br>
<br>

# NOTES
If the ESP32 stops responding, then just call a "`.reset()`" to the `picowireless`, to force it to reset.  
You can check if the ESP32 is responding by calling "`.get_fw_version()`"  
If you reset the ESP32, then you need to connect to WiFi and re-create all sockets again.

<br>
<br>

# TODO:
## Check if I/O definitions need cange:
In `wireless-pico/drivers/esp32spi/esp32spi.hpp` there are som definitions for "INPUT", "OUTPUT" and "INPUT_PULLUP".  
These have been changed for use with the ESP32-C3 as these are different.  
Check if thees give issues, or they can stay changed.
> I haven't used the GPIO function of the ESP32, so I don't know.
