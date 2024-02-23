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

#define CONFIG_BT_MAX_CONN 62
#define NB_CHAR 50

#define SCAN_INTERVAL 0x0640 /* 1000 ms */
#define SCAN_WINDOW   0x0030 /* 30 ms */
#define INIT_INTERVAL 0x0010 /* 10 ms */
#define INIT_WINDOW   0x0010 /* 10 ms */
#define CONN_INTERVAL 0x0320 /* 1000 ms */
#define CONN_LATENCY  0
#define CONN_TIMEOUT  	MIN(MAX((CONN_INTERVAL * 125 * \
			       		MAX(CONFIG_BT_MAX_CONN, 6) / 1000), 10), 3200)


/****************************************************************************/
/*								VARIABLES									*/
/****************************************************************************/
extern int first_attr;
extern bool ble_is_busy;
extern bool ble_is_advertising;
extern uint8_t envoi;
extern const void* read_value;//Pour passer du callback a write
extern struct bt_gatt_indicate_params ind_params;
extern struct bt_conn_auth_cb auth_cb_display;
// Multilink
extern struct bt_conn *conn_connecting;// A remplacer dans discover -> default_connv !
extern  uint8_t volatile conn_count;
extern bool volatile is_disconnecting;
extern struct bt_conn* connected_devices[CONFIG_BT_MAX_CONN]; //tableau de connections
extern int first_attr_array[CONFIG_BT_MAX_CONN]; //tableau de valeur du premier attribut de chaque connexion
extern struct bt_conn *info_conn;
// Discover
extern struct bt_uuid_16 discover_uuid;
extern struct bt_gatt_discover_params discover_params;
extern struct bt_gatt_subscribe_params subscribe_params;
// Indicate
extern struct Ble_Indicate indicate;

/****************************************************************************/
/*								STRUCTURES									*/
/****************************************************************************/

/**
 * @brief Structure used to pass void* through
 * differents read and write function
 * @param size The size of the object we want to send, with
 * a sizeof
 * @param data A pointer to the object's adress, to be generic
*/
struct Ble_Data {
    size_t size;
    void* data;
};
/**
 * @brief Structure used to get the value indicate by the peripheral.
 * You might use a cast to acess the data
 * @param data A pointer to the object's adress, to be generic
 * @param char_index An int which refer to the characteristic
 * @param conn_index An int which refer to the connection which
 * indicate, in order to know which peripheral is indicating
*/
struct Ble_Indicate{
	size_t size;
	void* data;
	int char_index;
	int conn_index;
};

/****************************************************************************/
/*						FONCTIONS UTILISATEUR								*/
/****************************************************************************/

/**
 * @brief This function is used to init the BLE
 * @param void
 * @return 1 if everything is okay, a zephyr error code else
*/
int ble_enable(void);

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

/*MULTILINK*/

/**
 * @brief 
 * This function is used to scan for nearby devices and connect to it by default.
 * @param 
 * -> {bt_le_scan_cb_t} cb, callback function activated when a device is found.
 * by default NULL : 
 * If NULL, then connect to device with strong enough RSSI (above -34db).  
 * else, you might put a function which has a connection criteria
 * @return
 * an error index. 0 if no error.
*/
int ble_scan_start(bt_le_scan_cb_t cb);

/**
 * @brief 
 * This function prints every connected devisces' MAC adress in the terminal
 * @param 
 * None. 
 * @return
 * -
*/
void ble_showconn();

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

void ble_discover_gatt_service(uint16_t service_UUID);

static uint8_t discover_func(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr,
                 struct bt_gatt_discover_params *params);

static bool eir_found(struct bt_data *data, void *user_data);

static void device_found(const bt_addr_le_t *addr, 
						 int8_t rssi, 
						 uint8_t type,
			 			 struct net_buf_simple *ad);

static void multilink_connected(struct bt_conn *conn, uint8_t reason);

static void multilink_disconnected(struct bt_conn *conn, uint8_t reason);

static uint8_t discover_func(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params);

static uint8_t indicate_received(struct bt_conn *conn,
			   struct bt_gatt_subscribe_params *params,
			   const void *data, uint16_t length);




#endif