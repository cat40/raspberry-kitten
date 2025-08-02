#include "utilities.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

void setup_digital_input(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_set_pulls(pin, true, false); // enable pullup
}

void setup_digital_output(uint pin, bool init_state)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, init_state);
}