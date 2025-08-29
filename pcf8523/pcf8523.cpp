#include "pcf8523.hpp"

extern "C"
{
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
}

#include <stdint.h>
#include <stdbool.h>

#define SECONDS_TENS_MASK   0x07
#define MINTUES_TENS_MASK   0x07
#define HOURS_TENS_MASK     0x03
#define DAYS_TENS_MASK      0x03
#define MONTHS_TENS_MASK    0x01
#define YEARS_TENS_MASK     0x0f

// register addresses and contents
#define PCF_READING_REGISTER    0x03

#define PCF_CONTROL_1_REGISTER  0x00
#define PCF_SIE                 (1<<2)
#define PCF_TMR_CLKOUT_CTRL     0x0f
#define PCF_PULSED_INTERRUPT    0x80

#define PCF_CONTROL_3_REGISTER  0x02
// battery switchover enabled, standard mode
#define PCF_BATTERY_ENABLED     0x00

Pcf8523::Pcf8523(uint8_t i2c_address, i2c_inst_t* i2c_instance)
{
    this->i2c_address = i2c_address;
    this->i2c = i2c_instance;
}

void Pcf8523::init(bool use_backup_battery)
{
    const static uint8_t RESET_SEQUENCE[] = {0x00, 0x58};
    const static uint8_t PULSED_INTERRUPT_SEQUENCE[] = {PCF_TMR_CLKOUT_CTRL, PCF_PULSED_INTERRUPT};
    i2c_write_blocking(i2c, i2c_address, RESET_SEQUENCE, 2, false);
    // set battery switchover
    if (use_backup_battery)
    {
        uint8_t CONTROl_3_SEQUENCE[] = {PCF_CONTROL_3_REGISTER, PCF_BATTERY_ENABLED};
        i2c_write_blocking(i2c, i2c_address, CONTROl_3_SEQUENCE, 2, false);
    }
}

void Pcf8523::get_reading(rtc_reading* reading)
{
   const static uint8_t register_address = PCF_READING_REGISTER;
    uint8_t buffer[7];
    i2c_write_blocking(i2c, i2c_address, &register_address, 1, true);
    i2c_read_blocking(i2c, i2c_address, buffer, 7, false);
    reading->second = bcd_to_decimal(buffer[0], SECONDS_TENS_MASK);
    reading->minute = bcd_to_decimal(buffer[1], MINTUES_TENS_MASK);
    reading->hour = bcd_to_decimal(buffer[2], HOURS_TENS_MASK);
    reading->day = bcd_to_decimal(buffer[3], DAYS_TENS_MASK);
    reading->weekday = buffer[4];
    reading->month = bcd_to_decimal(buffer[5], MONTHS_TENS_MASK);
    reading->year = bcd_to_decimal(buffer[6], YEARS_TENS_MASK);
    // reading->year = 25;  // until the register reads a valid value
    // printf("register readings: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
    // printf("register readings: %02u %02u %02u %02u %02u %02u %02u\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
}

void Pcf8523::set_time(rtc_reading* reading)
{
    // const static uint8_t register_address = PCF_READING_REGISTER;
    uint8_t buffer[8];
    buffer[0] = PCF_READING_REGISTER;
    buffer[1] = decimal_to_bcd(reading->second, SECONDS_TENS_MASK);
    buffer[2] = decimal_to_bcd(reading->minute, MINTUES_TENS_MASK);
    buffer[3] = decimal_to_bcd(reading->hour, HOURS_TENS_MASK);
    buffer[4] = decimal_to_bcd(reading->day, DAYS_TENS_MASK);
    buffer[5] = reading->weekday;  // todo get weekday from date to put here (set to 4 because I did this on a thursday)
    buffer[6] = decimal_to_bcd(reading->month, MONTHS_TENS_MASK);
    buffer[7] = decimal_to_bcd(reading->year, YEARS_TENS_MASK);
    // i2c_write_blocking(i2c, i2c_address, &register_address, 1, true);
    i2c_write_blocking(i2c, i2c_address, buffer, 8, true);
    // rtc_reading new_reading;
    // get_reading_pcf8523(&new_reading);
    // printf("New date reading is: ");
    // printf(DATE_FORMAT_STRING, new_reading.year, new_reading.month, new_reading.day, new_reading.hour, new_reading.minute, new_reading.second, 0);
}

void Pcf8523::init_interrupt_pin(uint interrupt_pin, gpio_irq_callback_t callback)
{
    gpio_init(interrupt_pin);
    gpio_pull_up(interrupt_pin);
    gpio_set_irq_enabled_with_callback(interrupt_pin, GPIO_IRQ_EDGE_FALL, true, callback);
}