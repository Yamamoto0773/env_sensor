#include "serial.h"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include <string.h>


void i2c_set_device_name(const char* dev_name);

int i2c_read(byte_t dev_addr, byte_t reg_addr, byte_t* data, int length);
int i2c_write(byte_t dev_addr, byte_t reg_addr, const byte_t* data, int length);
