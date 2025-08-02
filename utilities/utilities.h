#ifndef UTILITIES_H
#define UTILITIES_H

#include "pico/stdlib.h"
#include <stdbool.h>

#define get_time() to_ms_since_boot(get_absolute_time())

void setup_digital_input(uint pin);
void setup_digital_output(uint pin, bool init_state);


#endif