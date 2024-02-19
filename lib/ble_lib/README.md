# Librairie ble_lib
## About the project

The aim of this librairie is to simplify the BLE API provided by Zephyr, for our application. We will focalize on simplification for write and read messages, and connections management.

## Built with

[![](https://zephyrproject.org/wp-content/uploads/sites/38/2020/09/ZephyrLogo.png)](https://www.zephyrproject.org/)


## List of functions
### For the users
#### Ble activation
- `ble_enable()` allow you to enable the BLE of your board. Return 1 if everything is okay, a Zephyr error code else.
- `ble_disable()` allow you to disable the BLE of your board.

#### Advertising
- `ble_advertise_start_conn` allow you to start an advertising, and let your peripheral be connected. It's very useful when you want to connect a peripheral and a central.
- `ble_advertise_start_nconn` allow you you to start advertising, but your peripheral isn't connectable. It's very useful when you want to be a beacon for example, and just sending data, no matter what is received.

#### Indicate
- `ble_indicate` allow you to indicate a data in your GATT service. It's important to note that the indication is only done one time. If you want to have a custom service which send data on every change, you have to do it by hand. The indicate is very useful when the peripheral want to trigger the central is one specific data. As an example, you can indicate on a button interruption, to send a message to the central only when the button is pressed.


### Internals
- `write_fonction_callback` is used as a callback when a data is write to the device. This callback is generic and should be used with the `Ble_Data` struct provided by the library.
- `read_fonction_callback` is used as a callback when a data is read by the device. This callback is generic and should be used with the `Ble_Data` struct provided by the library.



## Knowing issues

Empty for the moment

## Contacts

| First Name        |  Last Name        |   Mail  
|---                |:-:                |:-:    
| Caldeira          | Quentin           | quentin89.caldeira@gmail.com 
| Brunet            | Jordan            | jordan.brunet12@gmail.com
| Pedroli           | Johan             | johan.pedroli@gmail.com
| Phung             | Billy             | billy.phung45@gmail.com

