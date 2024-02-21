#include "bond_periph.h"

static struct bt_le_adv_param adv_param;
static int bond_count;

void pairing_complete(struct bt_conn *conn, bool bonded)
{
	printk("Pairing completed. Rebooting...\n");
	sys_reboot(SYS_REBOOT_WARM);
}

static struct bt_conn_auth_info_cb bt_conn_auth_info = {
	.pairing_complete = pairing_complete
};

static void add_bonded_addr_to_filter_list(const struct bt_bond_info *info, void *data)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	bt_le_filter_accept_list_add(&info->addr);
	bt_addr_le_to_str(&info->addr, addr_str, sizeof(addr_str));
	bond_count++;
}

void ble_bond_init(void (*func)(const struct bt_bond_info *info, void *user_data), void *user_data){
	if (func == NULL)
	{
		bt_foreach_bond(BT_ID_DEFAULT, add_bonded_addr_to_filter_list, user_data);
	}
	else{
		bt_foreach_bond(BT_ID_DEFAULT, func, user_data);
	}

	bt_conn_auth_info_cb_register(&bt_conn_auth_info);
	adv_param = *BT_LE_ADV_CONN_NAME;
}

int ble_bond_start(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size){
	int err;
	err = bt_le_adv_stop();
	if (err) {
		return err;
	}

	adv_param.options &= ~(BT_LE_ADV_OPT_FILTER_CONN | BT_LE_ADV_OPT_FILTER_SCAN_REQ);
	err = bt_le_adv_start(&adv_param, ad, ad_size, sd, sd_size);

	if (err) {
		return err;
	}
	return 0;
}

int ble_bond_stop(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size){
	int err;
	err = bt_le_adv_stop();
	if (err) {
		return err;
	}
	adv_param.options |= (BT_LE_ADV_OPT_FILTER_CONN | BT_LE_ADV_OPT_FILTER_SCAN_REQ);
	err = bt_le_adv_start(&adv_param, ad, ad_size, sd, sd_size);

	if (err) {
		return err;
	}
	return 0;
}

int ble_bond_duration(uint8_t duration, const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size){
	int err;
	err = ble_bond_start(ad, ad_size, sd, sd_size);
	if(err != 0){
		return err;
	}
	bond_count = 0;
	while (bond_count == 0 && duration > 0){
		k_sleep(K_SECONDS(1));
		duration--;
	}
	err = ble_bond_stop(ad, ad_size, sd, sd_size);
	if(err != 0){
		return err;
	}
	return 0;
}

int ble_bond_1_conn(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size){
	int err;
	err = ble_bond_start(ad, ad_size, sd, sd_size);
	if(err != 0){
		return err;
	}
	bond_count = 0;
	while (bond_count == 0){
		k_sleep(K_SECONDS(1));
	}
	err = ble_bond_stop(ad, ad_size, sd, sd_size);
	if(err != 0){
		return err;
	}
	return 0;
}
