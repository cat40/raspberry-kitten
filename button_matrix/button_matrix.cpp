#include "button_matrix.hpp"

extern "C"
{
#include "utilities.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
}

#include <stdint.h>

// #define DEBUG_PRINTS

#ifdef DEBUG_PRINTS
#include <stdio.h>
#endif

ButtonMatrix::ButtonMatrix(const uint* rows, const uint* columns, const uint8_t num_rows, const uint8_t num_columns, uint32_t min_polling_interval):rows(rows),columns(columns),num_rows(num_rows),num_columns(num_columns),keymap(keymap)
{
//     this->rows = rows;
//     this->columns = columns;
//     this->num_rows = num_rows;
//     this->num_columns = num_columns;
//     this->keymap = keymap;
    this->min_polling_interval = min_polling_interval;
}

void ButtonMatrix::init(void)
{
    for (uint8_t i=0; i<num_columns; i++)
    {
        setup_digital_input(columns[i]);
    }
    for (uint8_t i=0; i<num_rows; i++)
    {
        setup_digital_output(rows[i], 1);
    }
}

int32_t ButtonMatrix::scan_button_matrix(void)
{
    static uint32_t time_since_last_poll = min_polling_interval+1;
    static signed char static_key;
    int8_t row=-1, column=-1;
    bool found_key = false;
    int32_t key;  // lsb is column, second lsb is row. Upper bytes are for error conditions

    if (to_ms_since_boot(get_absolute_time())-time_since_last_poll < min_polling_interval)
    {
        return 0;
    }

    time_since_last_poll = to_ms_since_boot(get_absolute_time());

    for (uint8_t i=0; i<num_rows; i++)
    {
        set_rows(1);
        gpio_put(rows[i], 0);
        int8_t temp_column = scan_columns();
        if (temp_column >= 0)
        {
            if(found_key)
            {
                key = -1;
                break;
            }
            column = temp_column;
            row = i;
            key = ((int32_t)row << 8) | column;
#ifdef DEBUG_PRINTS
            printf("found key, row %d, column %d\n", row, column);
            printf("char: %c\n", key);
#endif
            found_key = true;
        }
    }
    set_rows(1);  // was 0 for some reason?
    // if (key = -1) return -1;
    // check if no keys were pressed
    if (!found_key) key = 0;
#ifdef DEBUG_PRINTS
    printf("char2: %c\n", key_from_keymap(row, column));
#endif
    if (static_key == key)
    {
        return 0;
    }
    static_key = key;
    return key;
}

char ButtonMatrix::key_from_keymap(uint8_t row, uint8_t column)
{
    uint8_t index = num_columns*row + column;
    return keymap[index];
}

void ButtonMatrix::set_rows(bool value)
{
    for(int8_t i=0; i<num_rows; i++)
    {
        gpio_put(rows[i], value);
    }
}

int8_t ButtonMatrix::scan_columns(void)
{
    int8_t column = -1;
    for(int8_t i=0; i<num_columns; i++)
    {
        if(gpio_get(columns[i]) == 0)
        {
            if (column != -1) return -2;  // multiple keys pressed at once
            column = i;
        }
    }
    return column;
}