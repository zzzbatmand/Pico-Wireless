#pragma once

#include "pico/stdlib.h"
#include "drivers/esp32spi/esp32spi.hpp"

namespace pimoroni {

  class PicoWireless : public Esp32Spi {
    typedef Esp32Spi parent;
    
    //--------------------------------------------------
    // Methods
    //--------------------------------------------------
  public:
    virtual bool init();
    virtual bool init(uint8_t cs, uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t resetn, uint8_t gpio0, uint8_t ack);
  };

}
