#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"


// /*--- Fonction de callback qui s'exécute lorsqu'un device est détecté pendant le scan ----*/
// static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
//              struct net_buf_simple *ad)
// {
//     char dev[BT_ADDR_LE_STR_LEN];

//     /* Si le signal n'est pas assez fort (appareil trop loin), on passe */
//     if (rssi > -40){
//         bt_addr_le_to_str(addr, dev, sizeof(dev));
//         printk("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
//             dev, type, ad->len, rssi);

    
//         /* We're only interested in connectable events */
//         if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) {
//             int err = bt_le_scan_stop();
//             if (err) {
//                 printk("Stop LE scan failed (err %d)\n", err);
//             }

//             /*On essaye de se connecter à ce device puisqu'il envoie une donnée avec l'UUID qu'on cherche*/
//             err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, BT_LE_CONN_PARAM_DEFAULT, &conn_connecting);
//             if (err) {
//                 printk("Create connection failed (err %d)\n", err);
//                 /*Si erreur de connexion on recommence à scanner*/
//                 //scan_start();
//             }
//         }
//     }
// }


// /* ------------- Fontion qui active le scan avec certains paramètres ------------------ */
// static int scan_start(void)
// {
//     /* Use active scanning and disable duplicate filtering to handle any
//      * devices that might update their advertising data at runtime.
//      */
//     struct bt_le_scan_param scan_param = {
//         .type       = BT_LE_SCAN_TYPE_ACTIVE,
//         .options    = BT_LE_SCAN_OPT_NONE,
//         .interval   = BT_GAP_SCAN_FAST_INTERVAL,
//         .window     = BT_GAP_SCAN_FAST_WINDOW,
//     };

//     return bt_le_scan_start(&scan_param, device_found);
// }


struct bt_conn* connect;

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("ERROR connect\n");
	} else {
		printk("CONNECTED\n");
		ble_discover_gatt_service(BT_UUID_UDS_VAL);
		//connect=conn;
		//ble_scan_start(NULL);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
	ble_enable();
	//scan_start();
	ble_scan_start(NULL);
	while(1){
		k_msleep(1000); 
	}
	

	return 0;
}
