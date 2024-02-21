#include "multilink.h"





int main(){

    ble_enable();
    ble_scan_start(NULL);

    while(true){
        ble_showconn();
        k_sleep(K_MSEC(1000));
    }

    return 0;
}