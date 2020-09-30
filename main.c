#include "lps331ap.h"


int main(void) {

    const char* i2c_device_name = "/dev/i2c-1";
    const unsigned char i2c_address = 0x5d;

    if (connect_lps331ap(i2c_device_name, i2c_address) < 0) {
        return -1;
    }

    active_1hz();
    
    while (1) {
        float pressure_hpa;
        if (get_pressure(&pressure_hpa) < 0) continue;

        float temp_degc;
        if (get_temperature(&temp_degc) < 0) continue;
        
        printf("%8.3f [hPa]\t%4.1f [degC]\n", pressure_hpa, temp_degc);

        wait_for(1.0);
    }

    
    return 0;
}

