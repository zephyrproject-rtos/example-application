#include <ble_lib/ble_lib.h>

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/settings/settings.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/bluetooth/services/hrs.h>
#include <zephyr/bluetooth/services/ias.h>

#define BLE_DISABLE     0
#define BLE_ENABLE      1

typedef int ble_Status;
ble_Status ble_status=BLE_DISABLE;

int ble_enable(){
    int err;
    err=bt_enable(NULL);
    if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
    else{
        ble_status=BLE_ENABLE;
        return 1;
    }
    
}

int ble_advertise_start_conn(const struct bt_data* ad, 
                        size_t ad_size, 
                        const struct bt_data* sd,
                        size_t sd_size)
                        {
    while (ble_status==BLE_DISABLE) ble_enable();
    bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ad_size, sd, sd_size);
    return 0;
}

int ble_advertise_start_nconn(const struct bt_data* ad, 
                        size_t ad_size, 
                        const struct bt_data* sd,
                        size_t sd_size)
                        {
    while (ble_status==BLE_DISABLE) ble_enable();
    bt_le_adv_start(BT_LE_ADV_NCONN, ad, ad_size, sd, sd_size);
    return 0;
}