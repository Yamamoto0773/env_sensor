#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define PRESS_POUT_XL_REH 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A
#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C


typedef unsigned char byte_t;

byte_t i2c_read(int fd, byte_t reg) {
    byte_t buf[1];

    buf[0] = reg;

    if (write(fd, buf, 1) != 1) {
        perror("i2c_read");
    }
    if (read(fd, buf, 1) != 1) {
        perror("i2c_read");
    }

    return buf[0];
}

void i2c_write(int fd, byte_t reg, byte_t data) {
    byte_t buf[2];

    buf[0] = reg;
    buf[1] = data;

    if (write(fd, buf, 2) != 2) {
        perror("i2c_write");
    }
}

__s32 convert_from_complement(__u32 value, int bit_len) {
    if (value & (1 << (bit_len - 1))) {
        __u32 filter = 1;
        for (int i = 0; i < bit_len - 1; i++) {
            filter = (filter << 1) + 1;
        }

        return ((value ^ filter) + 1) * -1;
    } else {
        return value;
    }
}


int main(void) {
    const char* i2c_device_name = "/dev/i2c-1";
    const unsigned char i2c_address = 0x5d;

    int fd = open(i2c_device_name, O_RDWR);
    if (fd < 0) {
        perror("open device");
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
        perror("select slave");
        return -1;
    }

    // turn active, 1Hz output data rate
    i2c_write(fd, CTRL_REG1, 0x90);

    while (1) {
        __u32 press_xl = i2c_read(fd, PRESS_POUT_XL_REH);
        __u32 press_l = i2c_read(fd, PRESS_OUT_L);
        __u32 press_h = i2c_read(fd, PRESS_OUT_H);
        __u32 pressure = (press_h << 16) | (press_l << 8) | press_xl;     
        float pressure_hpa = convert_from_complement(pressure, 24) / 4096.0f;

        __u32 temp_h = i2c_read(fd, TEMP_OUT_H);
        __u32 temp_l = i2c_read(fd, TEMP_OUT_L);
        __u32 temp = (temp_h << 8) | temp_l;

        float temp_degc = 42.5f + convert_from_complement(temp, 16) / 480.0f;
        
        printf("%8.3f [hPa]\t%4.1f [degC]\n", pressure_hpa, temp_degc);

        sleep(1);
    }

    
    return 0;
}

