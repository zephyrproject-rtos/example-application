/** @file
 *  @brief HTS Service sample
 */

/*
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

void valeur_indicate(struct bt_conn *conn);

//Variable pour stocker les valeurs de la characteristic BT_UUID_GATT_DO du service BT_UUID_UDS
extern uint8_t attr_value[];

#ifdef __cplusplus
}
#endif