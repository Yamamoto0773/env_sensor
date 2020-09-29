#include "lps331ap.h"

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define PRESS_POUT_XL_REH 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A
#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C


float get_pressure(int fd) {
    __u32 press_xl = i2c_read(fd, PRESS_POUT_XL_REH);
    __u32 press_l = i2c_read(fd, PRESS_OUT_L);
    __u32 press_h = i2c_read(fd, PRESS_OUT_H);
    __u32 pressure = (press_h << 16) | (press_l << 8) | press_xl;     
    float pressure_hpa = convert_from_complement(pressure, 24) / 4096.0f;

    return pressure_hpa;
}

float get_temperature(int fd) {
    __u32 temp_h = i2c_read(fd, TEMP_OUT_H);
    __u32 temp_l = i2c_read(fd, TEMP_OUT_L);
    __u32 temp = (temp_h << 8) | temp_l;

    float temp_degc = 42.5f + convert_from_complement(temp, 16) / 480.0f;

    return temp_degc;
}

void turn_active_and_config(int fd) {
    // turn active, 1Hz output data rate
    i2c_write(fd, CTRL_REG1, 0x90);
}

