#include "serial.h"

#include <memory.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>


int connect_mhz19b(const char* device_name);

int get_co2();

void close_mhz19b();
