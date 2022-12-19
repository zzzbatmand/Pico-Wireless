#include "pico_wireless.hpp"
#include "drivers/esp32spi/spi_drv.hpp"

namespace pimoroni {

  bool PicoWireless::init() {
    bool success = parent::init();

    return success;
  }
  
  bool PicoWireless::init(uint8_t cs, uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t resetn, uint8_t gpio0, uint8_t ack) {
    bool success = parent::init(cs, sck, mosi, miso, resetn, gpio0, ack);

    return success;
  }
}
