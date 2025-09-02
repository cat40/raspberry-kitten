#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include "hardware/i2c.h"

class Eeprom
{
    public:
        Eeprom(i2c_inst_t* i2c, uint8_t address);
        void write_byte(uint16_t address, uint8_t data);
        void write_data(uint16_t address, uint8_t* data, uint32_t length);
        uint8_t read_byte(uint16_t address);
        void read_data(uint16_t address, uint8_t* data, uint32_t length);
    private:
        i2c_inst_t* i2c;
        uint8_t i2c_address;
        static void address_to_array(uint16_t address, uint8_t* array);
};

#endif