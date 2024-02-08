
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

#define DEVICE_NAME     "Zephyr Device"
#define ADV_INTERVAL    1000 // en millisecondes

void advertising_thread(void *p1, void *p2, void *p3)
{
    struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(
        BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_ONE_TIME,
        BT_GAP_ADV_FAST_INT_MIN_1, BT_GAP_ADV_FAST_INT_MAX_1);

    while (1) {
        int err;

        err = bt_le_adv_start(&adv_param, NULL, 0, NULL, 0);
        if (err) {
            printk("Advertising failed to start (err %d)\n", err);
            return;
        }

        printk("Advertising started\n");

        k_sleep(K_MSEC(ADV_INTERVAL));

        err = bt_le_adv_stop();
        if (err) {
            printk("Advertising failed to stop (err %d)\n", err);
            return;
        }

        printk("Advertising stopped\n");

        k_sleep(K_MSEC(ADV_INTERVAL));
    }
}

void main(void)
{
    int err;

    printk("Zephyr BLE Advertising Connectable Example\n");

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    k_thread_spawn(advertising_thread, NULL, NULL, NULL, 0, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

    while (1) {
        k_sleep(K_FOREVER);
    }
}
