

#include <ble_lib/ble_lib.h>
#include "data.h"





/*---------- Données présentes dans les données de l'advertising ----------*/
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};






int main(void)
{
	const char* data2="J1aaaaa";
	bool data=1;
	uint16_t data1=45;

	bt_enable(NULL);
	bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	/*Definition de la structure de callback liée a l'authentification*/
	bt_conn_auth_cb_register(&auth_cb_display);


	/*Envoi de la mesure en indicate*/
	while(1){
			ble_indicate_bis(&data, sizeof(data), userdata_svc, 1);
			k_msleep(1000);
			ble_indicate_bis(&data1, sizeof(data1), userdata_svc, 5);
			k_msleep(1000);
        
	};

	return 0;
}