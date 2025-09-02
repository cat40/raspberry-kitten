#include "eeprom.hpp"
      
Eeprom::Eeprom(i2c_inst_t* i2c, uint8_t address)
{
    this->i2c = i2c;
    this->i2c_address = address;
}

void Eeprom::write_byte(uint16_t address, uint8_t data)
{
    uint8_t address_array[2];
    address_to_array(address, address_array);
    i2c_write_blocking(i2c, i2c_address, address_array, 2, true);
    i2c_write_blocking(i2c, i2c_address, &data, 1, false);
}

void Eeprom::write_data(uint16_t address, uint8_t* data, uint32_t length)
{
    uint8_t address_array[2];
    address_to_array(address, address_array);
    i2c_write_blocking(i2c, i2c_address, address_array, 2, true);
    i2c_write_blocking(i2c, i2c_address, data, length, false);
}

uint8_t Eeprom::read_byte(uint16_t address)
{
    uint8_t address_array[2], byte;
    address_to_array(address, address_array);
    i2c_write_blocking(i2c, i2c_address, address_array, 2, true);
    i2c_read_blocking(i2c, i2c_address, &byte, 1, false);
    return byte;
}

void Eeprom::read_data(uint16_t address, uint8_t* data, uint32_t length)
{
    uint8_t address_array[2];
    address_to_array(address, address_array);
    i2c_write_blocking(i2c, i2c_address, address_array, 2, true);
    i2c_read_blocking(i2c, i2c_address, data, length, false);
}

static void address_to_array(uint16_t address, uint8_t* array)
{
    array[0] = address >> 8;
    array[1] = address & 0x00ff;
}