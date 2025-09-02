#ifndef BUTTON_MATRIX_H
#define BUTTON_MATRIX_H

extern "C" 
{
#include "pico/stdlib.h"
#include "hardware/gpio.h"
}

#include <stdbool.h>
#include <stdint.h>

class ButtonMatrix
{
    public:
        ButtonMatrix(const uint* rows, const uint* columns, const uint8_t num_rows, const uint8_t num_columns, uint32_t min_polling_interval);
        void init(void);
        // returns the key pressed, 0 if no keys pressed, and -1 if multiple keys pressed
        int32_t scan_button_matrix(void);  // note: this needs to be debounced. Limiting the polling rate is probably sufficent
        const uint *rows, *columns;
        const uint8_t num_rows, num_columns;
        const char* keymap;
        uint32_t min_polling_interval;
    private:
        // internal-use helper functions
        // char key_from_keymap(uint8_t row, uint8_t column);
        void set_rows(bool value);
        int8_t scan_columns(void);
};

#endif