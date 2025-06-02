#include "ht16k33.hpp"

extern "C"
{
#include "hardware/i2c.h"
}

#include <stdint.h>

// these defines copied from pico-examples, which presumably got them from the datasheet
#define HT16K33_SYSTEM_RUN      0x21
#define HT16K33_SET_ROW_INT     0xA0
#define HT16K33_DISPLAY_SETUP   0x80
#define HT16K33_DISPLAY_OFF     0x0
#define HT16K33_DISPLAY_ON      0x1
#define HT16K33_BRIGHTNESS      0xE0

#define HT16K33_MAX_BRIGHTNESS  15

Ht16k33::Ht16k33(uint8_t i2c_address, i2c_inst_t* i2c_instance, uint8_t num_digits)
{
    this->i2c_address = i2c_address;
    this->i2c_instance = i2c_instance;
    this->num_digits = num_digits;
}

void Ht16k33::init(void)
{
    write_byte(HT16K33_SYSTEM_RUN);
    write_byte(HT16K33_SET_ROW_INT);
    write_byte(HT16K33_DISPLAY_SETUP | HT16K33_DISPLAY_ON);
}

void Ht16k33::display_number_2_digit(uint8_t start_index, uint8_t number)
{
    // start_index = translate_index(start_index);
    char left, right;
    if (number > 99)
    {
        left = 'X';
        right = 'X';
    }
    else
    {
        left = (number / 10) + '0';
        right = (number % 10) + '0';
    }
    display_char(start_index, left);
    display_char(start_index-1, right);
}

void Ht16k33::display_char(uint8_t index, char character)
{
    display_char(index, character, false);
}

void Ht16k33::display_char(uint8_t index, char character, bool decimal_point)
{
    index = translate_index(index);
    uint8_t buffer[3];
    uint16_t binary = char_to_binary(character);
    if (decimal_point)
    {
        character |= (1<<14);
    }
    buffer[0] = index * 2;
    buffer[1] = binary & 0xff;
    buffer[2] = binary >> 8;
    i2c_write_blocking(i2c_instance, i2c_address, buffer, 3, false);
}

void Ht16k33::turn_off_display(uint8_t index)
{
    uint8_t buffer[3];
    buffer[0] = index * 2;
    buffer[1] = 0;
    buffer[2] = 0;
    i2c_write_blocking(i2c_instance, i2c_address, buffer, 3, false);
}

void Ht16k33::set_brightness(uint8_t brightness)  // todo see if this can be implimented per-display
{
    write_byte(HT16K33_BRIGHTNESS | (brightness <= 15 ? brightness : HT16K33_MAX_BRIGHTNESS));
}

void Ht16k33::write_byte(uint8_t byte)
{
    i2c_write_blocking(i2c_instance, i2c_address, &byte, 1, false);
}

uint16_t Ht16k33::char_to_binary(char character)
{
    // maps from pico-examples; apparantly they are standardish
    const static uint16_t capital_letters[] = {
        0xF7,0x128F,0x39,0x120F,0xF9,0xF1,0xBD,0xF6,0x1209,0x1E,0x2470,0x38,0x536,0x2136,
        0x3F,0xF3,0x203F,0x20F3,0x18D,0x1201,0x3E,0xC30,0x2836,0x2D00,0x1500,0xC09
    };
    const static uint16_t numbers[] = {
        0xC3F,0x406,0xDB,0x8F,0xE6,0xED,0xFD,0x1401,0xFF,0xE7
    };
    if (character >= '0' && character <= '9')
    {
        return numbers[character-'0'];
    }
    else if (character >= 'A' && character <= 'Z')
    {
        return capital_letters[character-'A'];
    }
    else
    {
        return 0;
    }
}