# Install firmware
To install the firmware on a standard Pico/Pico-W, just hold down the "BOOTSEL" button, while plugging in the USB cable.  
This should create a new storage device. (like a usb stick)

> You might need to mount it, depending on what distro you are running on.

Copy the firmware.uf2 file to the new storage device, and it should automatically eject and start running the firmware.

> You might need to unmount it manually, depending on what distro you are running on.

<br />
<br />

# Connect to the Pico
There are a few options to interface with the pico.  
But here are two options I can reccomend.  

## [Thonny](./Thonny.md)
**PRO:**
+ Easy to use.
+ Overview over files.
+ Easy to upload and download of both files and project.  

**CON:**
- No intellisense

<br>

## [VSCode](./VSCode.md)
**PRO:**
+ Run selection
+ Intellisense
+ Custom keyboard commands

**CON:**
- A bit hard to setup
- Minor issues with clearing the Pico
- Weird VSCode issues


<br>

# Example code
Example code can be found in the directory `"micropython/examples"`  