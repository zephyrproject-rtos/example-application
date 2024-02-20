#ifndef BLE_LIB_H
#define BLE_LIB_H

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



struct Ble_Data {
    size_t size;
    void* data;
};

int ble_enable();

int ble_advertise_start_conn(   const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);

int ble_advertise_start_nconn(  const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);

/*******************************************INTERNAL*****************************************************/

/* ----- Fonction de callback de la characteristic BT_UUID_GATT_DO qui s'exécute lors d'un read d'une centrale ----- */
ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);


/* ----- Fonction de callback de la characteristic BT_UUID_GATT_DO qui s'exécute lors d'un write d'une centrale ----- */
ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags);


/* --------------- Fonction qui verifie si la configuration du CCC change et l'indique ------------*/
void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value);





/* ------- Callback de l'indicate, s'exécute lorsque l'indicate est commencé -------------*/
void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err);


/* -------------Callback de l'indicate, s'exécute lorsque l'indicate est fini -----------*/
void indicate_destroy(struct bt_gatt_indicate_params *params);

/********************************************************************************************************/

int ble_indicate(struct Ble_Data* data, const struct bt_gatt_service_static svc, int offset);

ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags);

ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);

/*RW JORDAN*/
void write_result_callback(struct bt_conn *conn, 
                            uint8_t err,
				            struct bt_gatt_write_params *params);

int ble_write(struct bt_conn *conn, struct Ble_Data* value, int attr_handle);

uint8_t read_result_callback(struct bt_conn *conn, uint8_t err,
				            struct bt_gatt_read_params *params,
				            const void *data, uint16_t length);

int ble_read(struct bt_conn *conn, struct Ble_Data* value, int attr_handle);



#endif