#ifndef PCF8523_H
#define PCF8523_H

extern "C"
{
#include "pico/stdlib.h"
#include "hardware/i2c.h"
}

#include <stdint.h>

class Pcf8523
{
    public:
        Pcf8523(uint8_t i2c_address, i2c_inst_t* i2c_instance);
        void init(bool use_backup_battery);
        void get_reading(rtc_reading* reading);
        void set_time(rtc_reading* reading);
        void init_interrupt_pin(uint interrupt_pin, gpio_irq_callback_t callback);
    private:
        static inline uint8_t bcd_to_decimal(uint8_t bcd, uint8_t tens_mask) { return 10*((bcd >> 4) & tens_mask) + (bcd & 0x0f); }
        static inline uint8_t decimal_to_bcd(uint8_t decimal, uint8_t tens_mask) { return (((decimal/10)&tens_mask) << 4) | ((decimal % 10)&0x0f); }
        uint interrupt_pin;
        i2c_inst_t* i2c;
        uint8_t i2c_address;

};

typedef struct 
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
    uint8_t month;
    uint8_t year;
} rtc_reading;

#endif