#include "serial.h"

#include <linux/i2c-dev.h>

typedef unsigned char byte_t;

int i2c_connect(const char* i2c_device_name, unsigned char i2c_address);

byte_t i2c_read(int fd, byte_t reg);

void i2c_write(int fd, byte_t reg, byte_t data);
