# Building custom micropython firmware (ESP32 WiFi)
## Links and references
<!-- TODO: Update links -->
### WiFi firmware source:
https://github.com/pimoroni/pimoroni-pico
### Guide to build firmware:
https://github.com/virgesmith/pimoroni-pico/blob/main/micropython-build.md

<br/>

# Setting up the Pico SDK
**Step 1.** Create a new directory in your home directory to keep everything tidily in one place:
``` sh
cd ~
mkdir pico
cd pico
```

**Step 2.** Install the Pico SDK and examples:
``` sh
git clone https://github.com/raspberrypi/pico-sdk.git

# Set the PICO_SDK_PATH environment variable to where you just cloned the repo.
export PICO_SDK_PATH="$HOME/pico/pico-sdk"

# Download sub-modules.
cd pico-sdk
git submodule update --init
cd ..
```

**Step 3.** Install the toolchain needed to build Pico projects:
``` sh
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential ccache
```

**Step 4.** Install the MicroPython port. (Used for building micropython firmware):
``` sh
git clone https://github.com/micropython/micropython.git
cd micropython
# Update modules used by micropython.
# We will get 'lib/pico-sdk' and 'lib/tinyusb' as these are needed for the Pico.
git submodule update --init -- lib/pico-sdk lib/tinyusb

# Building the bytecode cross-compiler
make -C mpy-cross
cd ..
```

**Step 5.** Install the Wireless Pico libraries:
<!-- TODO: Upload to git -->
``` sh
git clone <TODO>
cd wireless-pico
git submodule update --init
cd ..
```

<br />

# Fixing the RTC
There is an error in the RP2 micropython, that causes the RTC to format date [wrong](https://forum.micropython.org/viewtopic.php?f=2&t=9601&start=10).  
To fix this, you have to edit the file at `micropython/ports/rp2/machine_rtc.c`
``` c
// Find the following struct:
mp_obj_t tuple[8] = {
    mp_obj_new_int(t.year),
    mp_obj_new_int(t.month),
    mp_obj_new_int(t.day),
    mp_obj_new_int(t.dotw),
    mp_obj_new_int(t.hour),
    mp_obj_new_int(t.min),
    mp_obj_new_int(t.sec),
    mp_obj_new_int(0)
};
// And replace that with the following:
mp_obj_t tuple[8] = {
    mp_obj_new_int(t.year),
    mp_obj_new_int(t.month),
    mp_obj_new_int(t.day),
    mp_obj_new_int(t.hour),
    mp_obj_new_int(t.min),
    mp_obj_new_int(t.sec),
    mp_obj_new_int(t.dotw),
    mp_obj_new_int(0)
};
// NOTE: Just move the "t.dotw" down to second-to-last.
```

<br />

# Enable REPL over UART
REPL (serial interface) is by default disabled over UART, and only available over USB.  
To enable this feature, edit the following file `micropython/ports/rp2/mpconfigport.h`
``` h
#define MICROPY_HW_ENABLE_UART_REPL             (0) // useful if there is no USB
// TO
#define MICROPY_HW_ENABLE_UART_REPL             (1) // useful if there is no USB
```

<br />

# Build Pico firmware
Just run the following commands in order, and it should just work.
``` sh
cd micropython/ports/rp2

# Prepare the Pico cmake files.
cmake -S . -B build-PICO -DPICO_BUILD_DOCS=0 -DUSER_C_MODULES=../../../wireless-pico/micropython/modules/micropython-pico-wireless.cmake -DMICROPY_BOARD=PICO -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
# NOTE: Replace "PICO" with "PICO_W" to build for PICO_W

# Make the actual firmware.
cmake --build build-PICO -j
```
The firmware shoud now be located at `"build-PICO/firmware.uf2"`

<br />

# Install firmware
To install the firmware on a standard Pico/Pico-W, just hold down the "BOOTSEL" button, while plugging in the USB cable.  
This should create a new storage device. (like a usb stick)  
Copy the firmware.uf2 file to the new storage device, and it should automatically eject and start running the firmware.

<br />

# Connect to the Pico
## Just follow the thonny guide at:  
https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico/2

<br />

## **(ALTERNATIVE)** Follow this guide
### Download and install [Thonny](https://thonny.org/).  
<br />

In the buttom right corner of Thonny, there should be som text saying `Python 3.9` or simmilar.  
![Thonny bar](./Other/thonny-status-bar-version.png)  
**NOTE:** It might say "`The same interpreter which runs Thonny (default)`"

<br />

Click the text, and a menu should appear.  
In the menu, select the option `"MicroPython (Raspberry Pi Pico)"`  
![Pico Python](./Other/thonny-micropython-pico-menu.png)

<br />

Next click the "STOP" button at the top of Thonny, to reset the Pico and start the console.  
![Thonny stop](./Other/thonny-stop.png)

<br />

The console should now be available at the buttom of thonny.  
![REPL](./Other/repl-connected.png)

<br />

# Custom ESP32 firmware
If the ESP32 pins isn't available, then a custom ESP32 firmware can be made.  
The [ESP32c3-firmware.md](./ESP32c3-firmware.md) file, shows how this is done.

<br />

# TODO:
## Check if I/O definitions need cange:
In `wireless-pico/drivers/esp32spi/esp32spi.hpp` thare are som definitions for "INPUT", "OUTPUT" and "INPUT_PULLUP".  
These have been changed for use with the ESP32C3 as these are different.  
Check if thees are needed to be modifyed, when using ESP-IDF instead of arduino!

## Note pin config
Different pins are used in the ESP32C3, a document on this chip specifically, should be made.
