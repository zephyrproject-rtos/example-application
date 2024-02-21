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







/****************************************************************************/
/*								STRUCTURES									*/
/****************************************************************************/

/**
 * @brief Structure used to pass void* through
 * differents read and write function
 * @param This structure contains the size of the object
 * we want to send (with a sizeof),
 * and a pointer to the object's adress, to be generic
*/
struct Ble_Data {
    size_t size;
    void* data;
};

/****************************************************************************/
/*						FONCTIONS UTILISATEUR								*/
/****************************************************************************/

/**
 * @brief This function is used to init the BLE
 * @param nothing
 * @return 1 if everything is okay, a zephyr error code else
*/
int ble_enable();

/**
 * @brief This function is used to disable the BLE
 * @param nothing
 * @return 1 if everything is okay, a zephyr error code else
*/
int ble_disable();


/**
 * @brief This function is used to start an advertise, an allow
 * your device to be connectable
 * @param A bt_data packet for advertising data, and for receiving data,
 * and their size. You might use ARRAY_SIZE(ad) provided by Zephyr, and not
 * sizeof, it will be wrong.
 * @return 1 if everything is okay, a zephyr error code else 
*/
int ble_advertise_start_conn(   const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);


/**
 * @brief This function is used to start an advertise, an allow
 * your device to be unconnectable. It's very useful when you just want
 * to spread data.
 * @param A bt_data packet for advertising data, and for receiving data,
 * and their size. You might use ARRAY_SIZE(ad) provided by Zephyr, and not
 * sizeof, it will be wrong.
 * @return 1 if everything is okay, a zephyr error code else 
*/
int ble_advertise_start_nconn(  const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);

/**
 * @brief This function is used when you want to do an indicate. It's
 * very useful when you want to trigger the central at a specific event.
 * @param You must used the struct declared above to send data, you need also the name
 * of the GATT service you want to refer to, and the offset is the associate characteristic.
 * As an example, if my service contains three characteristics, and I want to indicate my second
 * characteristic, my offset might be 2
 * @return 0 is the indicate action is busy
*/
int ble_indicate(struct Ble_Data* data, 
				 const struct bt_gatt_service_static svc, 
				 int offset);

/**
 * @brief This function is used when you want to write to a GATT server
 * @param You need the actual connection you target, the data you want to send,
 * by using the previous struct, and the handle. It's different as the number of characteristics,
 * because the handle may vary between two different communication.
 * But also, if you want to write to the second characteristic of the GATT server, you might put an
 * attr_handle of 2.
 * @return 1 is write is successful, -1 if write is failed, and 0 if there is no legal connection
*/
int ble_write(struct bt_conn *conn, 
			  struct Ble_Data* value, 
			  int attr_handle);

/**
 * @brief This function is used when you want to read to a GATT server
 * @param You need the actual connection you target, the data you want to send,
 * by using the previous struct, and the handle. It's different as the number of characteristics,
 * because the handle may vary between two different communication.
 * But also, if you want to write to the second characteristic of the GATT server, you might put an
 * attr_handle of 2.
 * @return 1 is read is successful, -1 if read is failed, and 0 if there is no legal connection	
*/		  			  
int ble_read(struct bt_conn *conn, 
			 struct Ble_Data* value, 
			 int attr_handle);

/****************************************************************************/
/*						FONCTIONS CACHEES API								*/
/****************************************************************************/

/**
 * @brief This function is callback, called when there is a read
 * on your GATT service
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
ssize_t read_fonction_callback(struct bt_conn *conn,
					    	   const struct bt_gatt_attr *attr,
					   		   void *buf, uint16_t len,
					    	   uint16_t offset);


/**
 * @brief This function is callback, called when there is a write
 * on your GATT service
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
ssize_t write_fonction_callback(struct bt_conn *conn,
					     	    const struct bt_gatt_attr *attr,
					     		const void *buf, uint16_t len,
					     		uint16_t offset, uint8_t flags);


/**
 * @brief This function is used to register your characteristic as an indicating
 * characteristic
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 	 uint16_t value);

/**
 * @brief This function is a callback, used when you start an indicate
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err);


/**
 * @brief This function is a callback, used when you finish an indicate
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
void indicate_destroy(struct bt_gatt_indicate_params *params);

/**
 * @brief This function is a callback, used when you start a write to 
 * a GATT server
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags);


/**
 * @brief This function is a callback, used when you start a read to 
 * a GATT server
 * @param The params are hide by a Zephyr layer, and they don't have to
 * bother you
 * @return Nothing for us
*/
ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);


/*A expliquer*/
/*RW JORDAN*/
void write_result_callback(struct bt_conn *conn, 
                            uint8_t err,
				            struct bt_gatt_write_params *params);



uint8_t read_result_callback(struct bt_conn *conn, uint8_t err,
				            struct bt_gatt_read_params *params,
				            const void *data, uint16_t length);





#endif