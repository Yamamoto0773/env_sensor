#include "i2c.h"


int i2c_connect(const char* i2c_device_name, unsigned char i2c_address) {
    
    int fd = open(i2c_device_name, O_RDWR);
    if (fd < 0) {
        perror("open device");
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
        perror("select slave");
        return -1;
    }

    return fd;
}

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
