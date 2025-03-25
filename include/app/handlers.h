#ifndef HANDLERS_H
#define HANDLERS_H

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

/* Handlers */
void handle_wifi_scan_result(struct net_mgmt_event_callback *cb);
void handle_wifi_connect_result(struct net_mgmt_event_callback *cb);
void handle_wifi_disconnect_result(struct net_mgmt_event_callback *cb);
void handle_ipv4_result(struct net_if *iface);

/* Semaphore Wrappers */
void ipv4_sem_take(void);
void ipv4_sem_give(void);

/* Event Handlers */
void ipv4_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface);
void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface);

#endif /* HANDLERS_H */