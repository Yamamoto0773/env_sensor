#include "lps331ap.h"


int main(void) {

    const char* i2c_device_name = "/dev/i2c-1";
    const unsigned char i2c_address = 0x5d;

    int fd = i2c_connect(i2c_device_name, i2c_address);

    if (fd < 0) {
        return -1;
    }

    turn_active_and_config(fd);
    
    while (1) {
        float pressure_hpa = get_pressure(fd);
        float temp_degc = get_temperature(fd);
        
        printf("%8.3f [hPa]\t%4.1f [degC]\n", pressure_hpa, temp_degc);

        wait_for(1.0);
    }

    
    return 0;
}

