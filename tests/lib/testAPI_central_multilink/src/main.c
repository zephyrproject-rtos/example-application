#include <ble_lib/ble_lib.h>



#define SORCIERE connected_devices[5]

int main(){

    ble_enable();
    ble_scan_start(NULL);
    
    while(true){
        ble_showconn();
        k_msleep(1000);
    }

    return 0;
}



