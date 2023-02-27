# Building custom micropython firmware (ESP32-NINA)
This guide will show you how to build the firmware for a Pico-Pi (RP2040).

<br>
<br>

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
<!-- TODO: Fix folder -->
``` sh
git clone https://github.com/zzzbatmand/Pico-Wireless
cd wireless-pico
git submodule update --init
cd ..
```

<!-- !! DEPRICATED !!
<br />

# Fixing the RTC
There is an error in the RP2 micropython, that causes the RTC to format date [wrong](https://forum.micropython.org/viewtopic.php?f=2&t=9601&start=10).  
To fix this, just follow the guide found [here](./Pico_RTC_Fix.md). -->

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

# Build submodules
# Only needed once
make BOARD=PICO submodules
# NOTE: Replace "PICO" with "PICO_W" to build for Pico-w

# Prepare the Pico cmake files.
cmake -S . -B build-PICO -DPICO_BUILD_DOCS=0 -DUSER_C_MODULES=../../../wireless-pico/micropython/modules/micropython-pico-wireless.cmake -DMICROPY_BOARD=PICO -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
# NOTE: Replace "-DMICROPY_BOARD=PICO" with "-DMICROPY_BOARD=PICO_W" to build for Pico-w

# Make the actual firmware.
cmake --build build-PICO -j
```
The firmware shoud now be located at `"build-PICO/firmware.uf2"`
