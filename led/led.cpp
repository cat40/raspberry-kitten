#include "led.hpp"

extern "C"
{
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
}

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

LED::LED(uint pin, float brightness)
{
    this->pin = pin;
    this->brightness = brightness;
    this->shadow_brightness = brightness;
    this->slice = pwm_gpio_to_slice_num(pin);
}

void LED::init(void)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_set_wrap(this->slice, this->wrap_resolution);
    pwm_set_gpio_level(pin, float_to_pwm_level(brightness));
}

void LED::off(void)
{
    if (brightness > 0)
    {
        shadow_brightness = brightness;
    }
    LED::set_brightness(0.0);
    busy_wait_ms(1);
    pwm_set_enabled(slice, false);
    // printf("LED %u off, shadow brightness = %f\n", pin, shadow_brightness);
}

void LED::on(void)
{
    LED::set_brightness(shadow_brightness);
    pwm_set_enabled(slice, true);
    // printf("LED %u off, shadow brightness = %f\n", pin, shadow_brightness);
}

void LED::set_brightness(float brightness)
{
    this->brightness = brightness;
    pwm_set_gpio_level(pin, float_to_pwm_level(brightness));
}

uint16_t LED::float_to_pwm_level(float input)
{
    // check bounds
    input = input > 1? 1 : input;
    input = input < 0? 0 : input;
    return (uint16_t) ((float)0xfe * input);
}