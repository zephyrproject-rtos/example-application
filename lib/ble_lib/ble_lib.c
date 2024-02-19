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

/********************************Internal*****************************************************************/

static uint8_t envoi;
static uint8_t indicating;


static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}
/* ---------------------------------------------------------------------------------*/

/* --- Définition fonction de callback pour l'évenement authentification cancel ----*/
static struct bt_conn_auth_cb auth_cb_display = {
	/*Si cancel de l'appairage, alors on exécute la fonction auth_cancel*/
	.cancel = auth_cancel,
};



//Indicate params
static struct bt_gatt_indicate_params ind_params;

//Variable de la characteristic BT_UUID_GATT_DO du service BT_UUID_UDS
uint8_t attr_value[] = { 10 };// A CHANGER POUR GENERIQUE
/*********************************************************************/





/* --------------- Fonction qui verifie si la configuration du CCC change et l'indique ------------*/
void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value)
{
	envoi = (value == BT_GATT_CCC_INDICATE) ? 1 : 0; 
	if (value == BT_GATT_CCC_INDICATE) {
        // Le client a activé les indications
        printk("Indications activées par le client\n");
    } else if (value == BT_GATT_CCC_NOTIFY) {
        // Le client a activé les notifications
        printk("Notifications activées par le client\n");
    } else {
        // Le client a désactivé les indications et les notifications
        printk("Indications et notifications désactivées par le client\n");
    }
}





/* ------- Callback de l'indicate, s'exécute lorsque l'indicate est commencé -------------*/
void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err)
{
	printk("Indication %s\n", err != 0U ? "fail" : "success");
}


/* -------------Callback de l'indicate, s'exécute lorsque l'indicate est fini -----------*/
void indicate_destroy(struct bt_gatt_indicate_params *params)
{
	//Une fois l'indicate fini on remet indicating=0 afin de pouvoir refaire un indicate.
	printk("Indication complete\n");
	indicating = 0U;
}

/*********************************************************************************************************/


int ble_enable(){
    int err;
    err=bt_enable(NULL);
    if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
    else{
        return 1;
    }
    
}

int ble_advertise_start_conn(const struct bt_data* ad, 
                        size_t ad_size, 
                        const struct bt_data* sd,
                        size_t sd_size)
                        {
    bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ad_size, sd, sd_size);
	bt_conn_auth_cb_register(&auth_cb_display);
    return 0;
}

int ble_advertise_start_nconn(const struct bt_data* ad, 
                        size_t ad_size, 
                        const struct bt_data* sd,
                        size_t sd_size)
                        {
    bt_le_adv_start(BT_LE_ADV_NCONN, ad, ad_size, sd, sd_size);
    return 0;
}

int ble_indicate(struct Ble_Data* data, const struct bt_gatt_service_static svc, int offset)
{
	if (envoi)
	{
		if (indicating)
		{
			return;
		}
		ind_params.attr = &svc.attrs[offset];
		ind_params.func = indicate_cb;
		ind_params.destroy = indicate_destroy;
		ind_params.data = data->data;
		ind_params.len = data->size;
		printk("INDICATE SIZE = %d\n",data->size);

		// On envoie la nouvelle valeur avec un indicate
		if (bt_gatt_indicate(NULL, &ind_params) == 0)
		{
			indicating = 1U;
		}
	}
}


/*peripheral callbacks*/



ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags)
						 /*A TESTER SUR 16 BITS POUR SIZEOF VOID* */
{
	struct Ble_Data *char_info = (struct Ble_Data *)attr->user_data;
	size_t to_copy = MIN(len, char_info->size - offset);
    if (to_copy > 0) {
        memcpy(char_info->data + offset, buf, to_copy);
        return to_copy;
    } else {
        return 0;
    }
}

ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset)
{
	struct Ble_Data *char_info = (struct Ble_Data *)attr->user_data;
	size_t to_copy = MIN(len, char_info->size - offset);
    if (to_copy > 0) {
        memcpy(buf, char_info->data + offset, to_copy);
        return to_copy;
    } else {
        return 0;
    }
};