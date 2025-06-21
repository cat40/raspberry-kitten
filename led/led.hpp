#ifndef LED_H
#define LED_H

extern "C"
{
#include "pico/stdlib.h"
}

#include <stdint.h>

class LED
{
    public:
        LED(uint pin, float brightness);
        void init(void);
        void set_brightness(float brightness);
        void on(void);
        void off(void);
        uint pin;
        float brightness;
    private:
        uint slice;
        float shadow_brightness;
        const static uint16_t wrap_resolution = 0xff;
        static uint16_t float_to_pwm_level(float input);
};

#endif