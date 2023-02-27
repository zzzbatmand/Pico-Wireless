# Burning Firmware
Try running the command `esptool.py` and see if the command is found, otherwie install the esptool.py at https://github.com/espressif/esptool


**Step 1.** Wipe Chip:
``` sh
esptool.py --chip esp32 --port <SERIAL PORT> erase_flash
```

**Step 2.** Install firmware:
``` sh
esptool.py --chip esp32 --port <SERIAL PORT> --baud 115200 write_flash 0 <FLASH FILE>.bin
```

<br />

# Building a custom ESP32 firmware
In case the default pins isn't available, then it's possible to build a custom version of the NINA-FW for the ESP32

To build the firmware, you can follow the guide at the [NINA-FW github](https://github.com/adafruit/nina-fw#building) but the guide is a bit out of date, as python3 needs to be used instead of pyton2.  
Alternatively the following is a more updated guide.

<br />

# Build the ESP32 toolchain
**Step 1.** Install the required Prerequisites:
``` sh
sudo apt install gcc git wget make libncurses-dev flex bison gperf python3 python3-pip python3-setuptools python3-serial python3-cryptography python3-future python3-pyparsing
```

**Step 2.** Install the ESP-IDF:  
The NINA-FW is build on an older version of esp-idf, so we need to use this version, untill further notice.
``` sh
mkdir -p ~/esp
cd ~/esp
git clone --branch v3.3.1 --recursive https://github.com/espressif/esp-idf.git
```

**Step 3.** Set up the tools:  
The NINA-FW is build on an older version of xtensa, so we need to use this version, untill further notice.
``` sh
# Download the xtensa image
wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
# Extract xtensa
tar -xzf xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
# Remove the tar ball
rm xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
```

**Step 4.** Set up the environment variables:  
This should be done every boot, whenever a build is needed, as this reset on boot.
``` sh
export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"
export IDF_PATH="$HOME/esp/esp-idf"
```

<br />

# Building the firmware
``` sh
cd ~/esp

# Download the source files.
git clone https://github.com/adafruit/nina-fw.git
cd nina-fw

# Build the firmware.
make firmware
```

A new file should appear named `NINA_W102-x.x.x.bin`  
To burn the firmware to the ESP32, use the reference at the top of this guide.

<br />

# Change pins
After cloning the firmware source from above, modify the file `"nina-fw/arduino/libraries/SPIS/src/SPIS.cpp"`  
At the buttom, you will see:
``` cpp
SPISClass SPIS(VSPI_HOST, 1, 14, 23, 18, 5, 33);
```
These are the pins, used for the SPI communication, and can be changed to fit your chip.  
The pin order can be found in the function definition:
``` cpp
SPISClass::SPISClass(spi_host_device_t hostDevice, int dmaChannel, int mosiPin, int misoPin, int sclkPin, int csPin, int readyPin)
```


<br />

# References
### ESP32 Toolcahin
https://docs.espressif.com/projects/esp-idf/en/v3.3.1/get-started/index.html#setup-toolchain
### NINA-FW
https://github.com/adafruit/nina-fw

https://www.youtube.com/watch?v=YjKEpQDj8S0