#include "i2c.h"


int get_pressure(float* out);

int get_temperature(float* out);

void active_1hz();

int connect_lps331ap(const char* device_name, const unsigned char i2c_address);

void close_lps331ap();

