# Connecting the PICO to the ESP32
## Install the ESP32 firmware.
Burn the ESP32 firmware, by following the guide at [NINA-Arduino](https://github.com/zzzbatmand/NINA-Arduino)  

<br>

## Pin mapping
```
PICO            ESP32
VSYS    <-->    VIN
GND     <-->    GND
GP7     <-->    D5  (CS)
GP10    <-->    D33 (_Ready)
GP11    <-->    EN  (_EN)
GP16    <-->    D23 (MISO)
GP18    <-->    D18 (SCK)
GP19    <-->    D14 (MOSI)
```
> **NOTE:** Your ESP32 pins may vary, just make sure that the SPI is connected correctly.  
> You can use ARDUINO to find this, by printing the SPI pins shown [here](Other/SPI-Pins/SPI-Pins.ino).

<br>

# Custom pins
You can specify the pins to use, when you call the `PicoWireless` constructer by calling it as follows.
```py
import picowireless

# Pins are defined as follows (cs, sck, mosi, miso, reset/_EN, gpio0, ack/_Ready)
wifi = picowireless.PicoWireless(7, 18, 19, 16, 11, 2, 10)
```

<br>

## Changing default pins
If you want to change the default pins instead, then this can be edited in `"drivers/esp32spi/spi_drv.hpp"`  
``` hpp
static const uint8_t DEFAULT_CS_PIN     = 7;
static const uint8_t DEFAULT_SCK_PIN    = 18;
static const uint8_t DEFAULT_MOSI_PIN   = 19;
static const uint8_t DEFAULT_MISO_PIN   = 16;
static const uint8_t DEFAULT_RESETN_PIN = 11;
static const uint8_t DEFAULT_GPIO0_PIN  = 2;
static const uint8_t DEFAULT_ACK_PIN    = 10;
```

<br>

# Custom ESP32 firmware
If the ESP32 pins isn't available, then a custom ESP32 firmware can be made at [NINA-Arduino](https://github.com/zzzbatmand/NINA-Arduino).
