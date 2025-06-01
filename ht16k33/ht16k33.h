#ifndef HT16K33_H
#define HT16K33_H

#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define HT16K33_DEFAULT_ADDRESS     0x70
#define HT16K33_DEFAULT_NUM_DIGITS  4

void init_ht16k33(void);  // note -- requires IO to be already initialized
void ht16k33_display_number_2_digit(uint8_t start_index, uint8_t number);
void set_ht16k33_i2c(i2c_inst_t* i2c_inst);
void set_ht16k33_address(uint8_t address);
void set_ht16k33_num_digits(uint8_t digits);
void ht16k33_display_char(uint8_t index, char character);
void ht16k33_turn_off_display(uint8_t index);
void ht16k33_set_brightness(uint8_t brightness);

#endif