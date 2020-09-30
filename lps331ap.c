#include "lps331ap.h"

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define PRESS_POUT_XL_REH 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A
#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C


int fd;


int get_pressure(float* out) {
    byte_t tmp;

    if (i2c_read(fd, PRESS_POUT_XL_REH, &tmp) < 0) return -1;
    __u32 press_xl = tmp;
    
    if (i2c_read(fd, PRESS_OUT_L, &tmp) < 0) return -1;
    __u32 press_l = tmp;
    
    if (i2c_read(fd, PRESS_OUT_H, &tmp) < 0) return -1;
    __u32 press_h = tmp;

    __u32 pressure = (press_h << 16) | (press_l << 8) | press_xl;     
    *out = convert_from_complement(pressure, 24) / 4096.0f;

    return 0;
}

int get_temperature(float* out) {
    byte_t buf;
    
    if (i2c_read(fd, TEMP_OUT_H, &buf) < 0) return -1;
    __u32 temp_h = buf;
    
    if (i2c_read(fd, TEMP_OUT_L, &buf) < 0) return -1;
    __u32 temp_l = buf;
    
    __u32 temp = (temp_h << 8) | temp_l;

    *out = 42.5f + convert_from_complement(temp, 16) / 480.0f;

    return 0;
}

void active_1hz() {
    // turn active, 1Hz output data rate
    i2c_write(fd, CTRL_REG1, 0x90);
}


int connect_lps331ap(const char* device_name, const unsigned char i2c_address) {
    fd = i2c_connect(device_name, i2c_address);

    if (fd < 0) {
        return -1;
    } else {
        return 0;
    }
}


void close_lps331ap() {
    close(fd);
}



