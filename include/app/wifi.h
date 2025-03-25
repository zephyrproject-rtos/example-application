#ifndef WIFI_H
#define WIFI_H

// K_SEM_DEFINE(wifi_connected, 0, 1);
// K_SEM_DEFINE(wifi_scan, 0, 1);

void wifi_connect(void);
void wifi_status(void);
void wifi_disconnect(void);
wifi_iface_status get_wifi_status(struct net_if* iface);

#endif /* WIFI_H */