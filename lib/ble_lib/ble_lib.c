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

/****************************************************************************/
/*								VARIABLES									*/
/****************************************************************************/
//int first_attr;
bool ble_is_busy;
bool ble_is_advertising;
uint8_t envoi;
const void* read_value;//Pour passer du callback a write
struct bt_gatt_indicate_params ind_params;
struct bt_conn_auth_cb auth_cb_display;
// Multilink
struct bt_conn *conn_connecting;// A remplacer dans discover -> default_connv !
uint8_t volatile conn_count;
bool volatile is_disconnecting;
struct bt_conn* connected_devices[CONFIG_BT_MAX_CONN]; //tableau de connections
int first_attr_array[CONFIG_BT_MAX_CONN];
struct bt_conn *info_conn;
// Discover
struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
struct bt_gatt_discover_params discover_params;
struct bt_gatt_subscribe_params subscribe_params_array[NB_CHAR];
int char_index = 0;

bt_le_scan_cb_t* save_cb_scan;
/* Mutex servant a gérer les permissions de lecture/ecriture*/
struct k_mutex ble_bus;
/* Mutex servant a empecher les actions BLE si la connexion n'est pas effectuée*/
struct k_mutex is_connected;

struct Ble_Indicate indicate={
    .char_index=-1,
    .conn_index=-1,
    .data=NULL,
};

/****************************************************************************/
/*						FONCTIONS UTILISATEUR								*/
/****************************************************************************/




int ble_enable(){
    int err;
    err=bt_enable(NULL);//Fonction Zephyr
    if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return err;
	}
    else{
        return 1;
    }
    
}

int ble_disable(){
    int err;
    err=bt_disable();//Fonction Zephyr
    if (err) {
		printk("Bluetooth disabled failed (err %d)\n", err);
		return err;
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
    int err=bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ad_size, sd, sd_size);
	bt_conn_auth_cb_register(&auth_cb_display);//Permet de gérer l'authenfication de
    //Manière cachée dans le cas d'un device connectable
    if (err) {
		printk("Bluetooth advertise init failed (err %d)\n", err);
		return err;
	}
    else{
        return 1;
    }
}

int ble_advertise_start_nconn(const struct bt_data* ad, 
                        size_t ad_size, 
                        const struct bt_data* sd,
                        size_t sd_size)
                        {
    int err=bt_le_adv_start(BT_LE_ADV_NCONN, ad, ad_size, sd, sd_size);
    if (err) {
		printk("Bluetooth advertise nit failed (err %d)\n", err);
		return err;
	}
    else{
        return 1;
    }
}

int ble_indicate(struct Ble_Data* data, const struct bt_gatt_service_static svc, int offset)
{
	if (envoi) //Si le cb ccf_changed est configuré
	{

		ind_params.attr = &svc.attrs[1+(4*offset)];
        //1+4*offset, c'est car le premier attr est pour le service,
        //et ensuite on incrémente de 4 en 4, car characteristic prend 2 attr,
        // CPF prend 1 attr, et CCC prend 1 attr. On fait -4 pour que
        //l'utilisateur commence a déclarer ses caractéristiques a 1 et pas 0
		ind_params.func = indicate_cb;
		ind_params.destroy = indicate_destroy;
		ind_params.data = data->data;
		ind_params.len = data->size;

		// On envoie la nouvelle valeur avec un indicate
		if (bt_gatt_indicate(NULL, &ind_params) == 0)
		{
		}
	}
}

int ble_read(struct bt_conn *conn, struct Ble_Data* value, int attr_handle)
{
    if (conn) {
        // Incrémenter la référence de la connexion pour s'assurer qu'on est connecté
        bt_conn_ref(conn);
        // Création des paramètres de lecture
        static struct bt_gatt_read_params read_params;

        struct bt_uuid pt_uuid;
        /*Definition du type d'UUID*/
        pt_uuid.type = BT_UUID_TYPE_16;
        /* Fonction de callback qui s'éxecute après la lecture*/
        read_params.func = read_result_callback;
        /*Nomre de valeur qu'on va lire*/
        read_params.handle_count = 1;
        /*Handle qu'on veut lire*/
        read_params.single.handle = first_attr_array[bt_conn_index(conn)] + (4*attr_handle);
        /*Offset*/
        read_params.single.offset= 0;
        /*Pointeur vers le type d'UUID qu'on a défini*/
        read_params.by_uuid.uuid = &pt_uuid;			

        /*Fonction pour lire une données du périphérique*/	
        if ((bt_gatt_read(conn, &read_params) == 0)){
            printk("Read succesfull\n");
            bt_conn_unref(conn); 
            for (size_t i = 0; i < value->size; ++i) {
                ((unsigned char*)value->data)[i] = ((const unsigned char*)read_value)[i];
            }
            return 1;
        }
        else{
            printk("Read failed\n");
            bt_conn_unref(conn); 
            return -1;
        }
    }
    else {
        return 0;
    }
}

int ble_write(struct bt_conn *conn, struct Ble_Data* value, int attr_handle){
    // Vérifier si la connexion est valide
    if (conn) {
        //Declaration of write params
        static struct bt_gatt_write_params write_params;
        /*Incrémenter la référence de la connexion pour s'assurer qu'on est connecté*/
        bt_conn_ref(conn);

        /*-----------Mise à jour des paramètres d'écriture------------*/
        /*Fonction de callback pour l'écriture*/
        write_params.func = write_result_callback;
        /*Handle de l'attribute que l'on souhaite écrire*/
        write_params.handle = first_attr_array[bt_conn_index(conn)] + (4*attr_handle);
        /*Offset auquel on souhaite écrire la valeur (position, par exemple si la taille est de 5 octets et qu'on souhaite écrire le 3ème on met 2)*/
        write_params.offset = 0;
        /*Valeur à écrire*/
        write_params.data = value->data;
        /*Taille de la valeur à écrire*/
        write_params.length = value->size;

        /*Fonction pour écrire un attribut du périphérique*/
        if ((bt_gatt_write(conn, &write_params) == 0)) {
            printk("Write succesfull\n");
            // Décrémenter la référence de la connexion
            bt_conn_unref(conn); 
            return 1;
        } 
        else {
            printk("Write failed\n");
            // Décrémenter la référence de la connexion
            bt_conn_unref(conn); 
            return -1;
        }        
    }
    else {
        return 0;
    }
}

int ble_scan_start(bt_le_scan_cb_t* cb){
    /*Scan parameter*/
	struct bt_le_scan_param scan_param = { 
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;
    save_cb_scan=cb;
    /*Default callback*/
    if(save_cb_scan==NULL)
    {
	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
    printk("Scanning with DEFAULT connection callback\n");
    }

    /*Custom callback*/
    else{
	err = bt_le_scan_start(&scan_param, cb);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
    printk("Scanning with CUSTOM connection callback\n");
    }

	return err;
}

void ble_showconn(){
    int i;
    char addr[BT_ADDR_LE_STR_LEN];
    for(i=0;i<conn_count;i++) {
        bt_addr_le_to_str(bt_conn_get_dst(connected_devices[i]), addr, sizeof(addr));
        printk("Device %d : %s\n",i,addr);
    }

}

void ble_discover_gatt_service(uint16_t service_UUID)
{
    int err;
    memcpy(&discover_uuid, BT_UUID_DECLARE_16(service_UUID), sizeof(discover_uuid));
    discover_params.uuid = &discover_uuid.uuid;
    discover_params.func = discover_func;
    discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
    discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
    discover_params.type = BT_GATT_DISCOVER_PRIMARY;

    err = bt_gatt_discover(conn_connecting, &discover_params);
    if (err) {
        printk("Discover failed(err %d)\n", err);
        return;
    }
}

/****************************************************************************/
/*						FONCTIONS PRIVEES API								*/
/****************************************************************************/

void write_result_callback(struct bt_conn *conn, uint8_t err,
				     struct bt_gatt_write_params *params)
{
    if (err){
		return 0;
	}
	printk("Write callback OK\n");
};

uint8_t read_result_callback(struct bt_conn *conn, uint8_t err,
				    struct bt_gatt_read_params *params,
				    const void *data, uint16_t length)
{
	if (err){
		return 0;
	}
    read_value = data;
    printk("Read callback OK\n");
	return BT_GATT_ITER_STOP;
};

void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}

struct bt_conn_auth_cb auth_cb_display = {
	/*Si cancel de l'appairage, alors on exécute la fonction auth_cancel*/
	.cancel = auth_cancel,
};

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

void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err)
{
	printk("Indication %s\n", err != 0U ? "fail" : "success");
}

void indicate_destroy(struct bt_gatt_indicate_params *params)
{
	//Une fois l'indicate fini on remet indicating=0 afin de pouvoir refaire un indicate.
	printk("Indication complete\n");
}

ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags)						 
{
	struct Ble_Data *char_info = (struct Ble_Data *)attr->user_data;
	size_t to_copy = MIN(len, char_info->size - offset);
    if (to_copy > 0) {
        memcpy(char_info->data + offset, buf, to_copy);//On copie la
        //data de la structure dans le buffer d'envoi
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
	printk("READ\n");
    if (to_copy > 0) {
        memcpy(buf, char_info->data + offset, to_copy);// On copie
        //le contenu du buffer de reception dans la data de la structure
        return to_copy;
    } else {
        return 0;
    }
};

static uint8_t discover_func(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr,
                 struct bt_gatt_discover_params *params)
{
    if (!attr) {
        printk("Discover complete\n");
        (void)memset(params, 0, sizeof(*params));
		if(conn_count<CONFIG_BT_MAX_CONN) ble_scan_start(save_cb_scan);
        return BT_GATT_ITER_STOP;
    }

    int err;

    if (discover_params.type != BT_GATT_DISCOVER_CHARACTERISTIC){
        first_attr_array[bt_conn_index(conn)] = (attr->handle) + 2;
        struct bt_gatt_subscribe_params *subscribe_params_test = &subscribe_params_array[char_index];
        subscribe_params_test->value_handle = (attr->handle) + 2;
        subscribe_params_test->notify = indicate_received;
        subscribe_params_test->value = BT_GATT_CCC_INDICATE;
        subscribe_params_test->ccc_handle = (attr->handle) + 3;

        err = bt_gatt_subscribe(conn, subscribe_params_test);
        if (err && err != -EALREADY) {
            printk("Subscribe failed (err %d)\n", err);
        } 
        else {
            printk("[SUBSCRIBED]\n");
        }

        discover_params.uuid = NULL;
        discover_params.start_handle = (attr->handle) + 4;
        discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
        discover_params.func = discover_func;
        discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;

        err = bt_gatt_discover(conn, &discover_params);
        if (err) {
            printk("Discover failed(err %d)\n", err);
            return;
        }
    }
    else{
        struct bt_gatt_subscribe_params *subscribe_params_test = &subscribe_params_array[char_index];
        subscribe_params_test->value_handle = (attr->handle) + 1;
        subscribe_params_test->notify = indicate_received;
        subscribe_params_test->value = BT_GATT_CCC_INDICATE;
        subscribe_params_test->ccc_handle = (attr->handle) + 2;

        err = bt_gatt_subscribe(conn, subscribe_params_test);
        if (err && err != -EALREADY) {
            printk("Subscribe failed (err %d)\n", err);
        } 
        else {
            printk("[SUBSCRIBED]\n");
        }

        discover_params.uuid = NULL;
        discover_params.start_handle = (attr->handle) + 1;
        discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
        discover_params.func = discover_func;
        discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;

        err = bt_gatt_discover(conn, &discover_params);
        if (err) {
            printk("Discover failed(err %d)\n", err);
            return;
        }
        
    }
    char_index++;
    return BT_GATT_ITER_STOP;
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	struct bt_conn_le_create_param create_param = {
		.options = BT_CONN_LE_OPT_NONE,
		.interval = INIT_INTERVAL,
		.window = INIT_WINDOW,
		.interval_coded = 0,
		.window_coded = 0,
		.timeout = 0,
	};
	struct bt_le_conn_param conn_param = {
		.interval_min = CONN_INTERVAL,
		.interval_max = CONN_INTERVAL,
		.latency = CONN_LATENCY,
		.timeout = CONN_TIMEOUT,
	};
	char addr_str[BT_ADDR_LE_STR_LEN];

	if (conn_connecting) {
		return;
	}
	//printk("dans device found\n");
	if (rssi > -34){ //ajout filtre puissance signal

	/*Display scanned devices informations*/
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

	/* We're only interested in connectable events */
	if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) 
		{
			int err = bt_le_scan_stop();
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
			}

			/*On essaye de se connecter à ce device puisqu'il envoie une donnée avec l'UUID qu'on cherche*/
			err = bt_conn_le_create(addr, &create_param, &conn_param, &conn_connecting);
			if (err) {
				printk("Create connection failed (err %d)\n", err);
				ble_scan_start(save_cb_scan); //Retry scan with default parameters if failed
			}
		return;
	}

	}
}

static void multilink_connected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err; //ajout variable de test erreur

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (reason) {
		printk("Failed to connect to %s (%u)\n", addr, reason);

		bt_conn_unref(conn_connecting);
		conn_connecting = NULL;

		ble_scan_start(save_cb_scan);
		return;
	}

	printk("Connected (%u): %s\n", conn_count, addr);

	/*Récupération de la connexion*/
	info_conn = conn;

	if (conn == conn_connecting) {
		connected_devices[conn_count]= conn; //ajout de l'objet conn dans le tabeau des connections
	}

	conn_connecting = NULL;
	conn_count++;
}

static void multilink_disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	bt_conn_unref(conn);

	if ((conn_count == 1U) && is_disconnecting) {
		is_disconnecting = false;
		ble_scan_start(save_cb_scan);
	}
    for(int i=bt_conn_index(conn)+1; i<conn_count; i++){
        connected_devices[i]=connected_devices[i+1];
        first_attr_array[i]=first_attr_array[i+1];
    }
	conn_count--;
}

static uint8_t indicate_received(struct bt_conn *conn,
               struct bt_gatt_subscribe_params *params,
               const void *data, uint16_t length)
{
    if (!data) {
        printk("[UNSUBSCRIBED]\n");
        params->value_handle = 0U;
        return BT_GATT_ITER_STOP;
    }
    indicate.data=data;
    indicate.conn_index=bt_conn_index(conn);
    indicate.char_index=(params->value_handle);//AJOUTER FIRST ATTR
    printk("Donnée reçue : attr %d\n", params->value_handle);
    return BT_GATT_ITER_CONTINUE;
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = multilink_connected,
	.disconnected = multilink_disconnected,
};
