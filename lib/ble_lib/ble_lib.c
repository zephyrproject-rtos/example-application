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

bool ble_is_busy;
bool ble_is_advertising;
static uint8_t envoi;
const void* read_value;//Pour passer du callback a write
static struct bt_gatt_indicate_params ind_params;
static struct bt_conn_auth_cb auth_cb_display;

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
        ble_is_advertising=true;
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
        ble_is_advertising=true;
        return 1;
    }
}

int ble_indicate(struct Ble_Data* data, const struct bt_gatt_service_static svc, int offset)
{
	if (envoi) //Si le cb ccf_changed est configuré
	{
		if (ble_is_busy)//On teste si le bus BLE est libre
		{
			return 0;
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
			ble_is_busy = true; //On bloque le bus BLE
		}
	}
}

int ble_read(struct bt_conn *conn, struct Ble_Data* value, int attr_handle)
{
    if (ble_is_busy) return 0;
    if (conn) {
        // Incrémenter la référence de la connexion pour s'assurer qu'on est connecté
        bt_conn_ref(conn);
        ble_is_busy = true;
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
        read_params.single.handle = attr_handle;
        /*Offset*/
        read_params.single.offset= 0;
        /*Pointeur vers le type d'UUID qu'on a défini*/
        read_params.by_uuid.uuid = &pt_uuid;			

        /*Fonction pour lire une données du périphérique*/	
        if ((bt_gatt_read(conn, &read_params) == 0)){
            printk("Read succesfull\n");
            bt_conn_unref(conn); 
            printk("Read value :%s\n\n", ((char*)read_value));
            for (size_t i = 0; i < value->size; ++i) {
                ((unsigned char*)value->data)[i] = ((const unsigned char*)read_value)[i];
            }
            return 1;
        }
        else{
            printk("Read failed\n");
            bt_conn_unref(conn); 
            ble_is_busy = false;
            return -1;
        }
    }
    else {
        ble_is_busy = false;
        return 0;
    }
}

int ble_write(struct bt_conn *conn, struct Ble_Data* value, int attr_handle){
    // Vérifier si la connexion est valide
    if (ble_is_busy) return 0;
    if (conn) {
        //Declaration of write params
        static struct bt_gatt_write_params write_params;
        /*Incrémenter la référence de la connexion pour s'assurer qu'on est connecté*/
        bt_conn_ref(conn);
        ble_is_busy = true;

        /*-----------Mise à jour des paramètres d'écriture------------*/
        /*Fonction de callback pour l'écriture*/
        write_params.func = write_result_callback;
        /*Handle de l'attribute que l'on souhaite écrire*/
        write_params.handle = attr_handle;
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
            ble_is_busy = false;
            return -1;
        }        
    }
    else {
        ble_is_busy = false;
        return 0;
    }
}

/****************************************************************************/
/*						FONCTIONS CACHHES API								*/
/****************************************************************************/

void write_result_callback(struct bt_conn *conn, uint8_t err,
				     struct bt_gatt_write_params *params)
{
	printk("Write value : %d\n", *((uint8_t*)params->data));
    ble_is_busy = false;// On libere le bus car le write est fini
};

uint8_t read_result_callback(struct bt_conn *conn, uint8_t err,
				    struct bt_gatt_read_params *params,
				    const void *data, uint16_t length)
{
    printk("Read callback\n");
	if (err){
		return 0;
	}
	read_value = data;
	printk("Read result : %d\n", *((uint8_t*)read_value));
    ble_is_busy = false;
	return BT_GATT_ITER_STOP;
};

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
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
	ble_is_busy = false;
}

ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags)						 
{
    if (ble_is_busy) return 0;
    ble_is_busy = true;//On bloque le bus
	struct Ble_Data *char_info = (struct Ble_Data *)attr->user_data;
	size_t to_copy = MIN(len, char_info->size - offset);
    if (to_copy > 0) {
        memcpy(char_info->data + offset, buf, to_copy);//On copie la
        //data de la structure dans le buffer d'envoi
        ble_is_busy = false;
        return to_copy;
    } else {
        ble_is_busy = false;//On libere le bus a la fin
        return 0;
    }
}

ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset)
{
    if (ble_is_busy) return 0;
	struct Ble_Data *char_info = (struct Ble_Data *)attr->user_data;
	size_t to_copy = MIN(len, char_info->size - offset);
	printk("READ\n");
    if (to_copy > 0) {
        memcpy(buf, char_info->data + offset, to_copy);// On copie
        //le contenu du buffer de reception dans la data de la structure
        ble_is_busy = false;
        return to_copy;
    } else {
        ble_is_busy = false;
        return 0;
    }
};