#ifndef HT16K33_H
#define HT16K33_H

#include <stdint.h>

extern "C"
{
#include "pico/stdlib.h"
#include "hardware/i2c.h"
}

#define HT16K33_DEFAULT_ADDRESS     0x70

class Ht16k33
{
    public:
        Ht16k33(uint8_t i2c_address, i2c_inst_t* i2c_instance, uint8_t num_digits);
        void init(void);  // note -- requires IO to be already initialized
        void display_number_2_digit(uint8_t start_index, uint8_t number);
        void display_char(uint8_t index, char character);
        void display_char(uint8_t index, char character, bool decimal_point);
        void turn_off_display(uint8_t index);
        void set_brightness(uint8_t brightness);
        uint8_t i2c_address;
        i2c_inst_t* i2c_instance;
        uint8_t num_digits;
    private:
        void write_byte(uint8_t byte);
        static uint16_t char_to_binary(char character);
        static inline uint8_t translate_index(uint8_t index) { return 3-index; }
};

#endif