#include "serial.h"

#include <linux/i2c-dev.h>

int i2c_connect(const char* i2c_device_name, unsigned char i2c_address);

int i2c_read(int fd, byte_t reg, byte_t* out);

int i2c_write(int fd, byte_t reg, byte_t data);
