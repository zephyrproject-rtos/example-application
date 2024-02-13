.. _peripheral_ht:

Bluetooth: Peripheral HT
########################

Overview
********

Similar to the :ref:`Peripheral <ble_peripheral>` sample, except that this
application specifically exposes the HT (Health Thermometer) GATT Service.

On Nordic nRF devices, this sample uses the built-in TEMP peripheral to return
die temperature values. On other boards, it will generate dummy temperature
values.


Requirements
************

* BlueZ running on the host, or
* A board with BLE support

Building and Running
********************

This sample can be found under :zephyr_file:`samples/bluetooth/peripheral_ht` in the
Zephyr tree.

See :ref:`bluetooth samples section <bluetooth-samples>` for details.
