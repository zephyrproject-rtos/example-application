# Application peripheral_bond
## About the project

The aim of this application is to test bonding.  
For that, if you push button, you will open connection for unkown device. We use **ble_bond_1_conn()**. Until, there is no connection, it's open.

## Built with

[![](https://zephyrproject.org/wp-content/uploads/sites/38/2020/09/ZephyrLogo.png)](https://www.zephyrproject.org/)

## Function's list
- **ble_bond_init(void (\*func)(const struct bt_bond_info \*info, void \*user_data), void\* user_data)**
    * Aim : Redirects pairing request to function  
For the moment, user can change function but may be, it's too complicated.
    * No input
    * No output
- **ble_bond_start(const struct bt_data\* ad, int ad_size, const struct bt_data\* sd, int sd_size)**
    * Aim : Starts bonding
    * No input
    * Output : 0 if no error or error code
- **ble_bond_stop(const struct bt_data\* ad, int ad_size, const struct bt_data\* sd, int sd_size)**
    * Aim : Stops bonding
    * No input
    * Output : 0 if no error or error code
- **ble_bond_duration(uint8_t duration, const struct bt_data\* ad, int ad_size, const struct bt_data\* sd, int sd_size)**
    * Aim : Activates bonding during argument time or until there isn't connection.
    * Input : Maximum seconds of bonding
    * Output : 0 if no error or error code
- **ble_bond_1_conn(const struct bt_data\* ad, int ad_size, const struct bt_data\* sd, int sd_size)**
    * Aim : Activates bonding until there isn't connection.
    * No input
    * Output : 0 if no error or error code


## Requirements
- Adjust prj.conf
- Add every library
- 2 global variables
```c
static struct bt_le_adv_param adv_param;
static int bond_count;
```
- Call *ble_bond_param()* before other functions of bonding
- Function of callback
```c
static struct bt_conn_auth_info_cb bt_conn_auth_info = {
	.pairing_complete = pairing_complete
};
```
## Knowing issues

Empty for the moment

## Information
if you don't want to have random mac address, you can disable this parameter :
```
CONFIG_BT_PRIVACY=y
```

## Contacts

| First Name        |  Last Name        |   Mail  
|---                |:-:                |:-:    
| Caldeira          | Quentin           | quentin89.caldeira@gmail.com 
| Brunet            | Jordan            | jordan.brunet12@gmail.com
| Pedroli           | Johan             | johan.pedroli@gmail.com
| Phung             | Billy             | billy.phung45@gmail.com

4C:EB:BD:1A:B5:13