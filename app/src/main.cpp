#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <iostream>
#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

int main(void)
{
    // struct net_if *iface = net_if_get_default();

    /* Main Function */
    printk("Hello World!\n");

    // if (iface)
    // {
    //     net_if_up(iface);
    //     printk("Network interface found!\n");
    // }
    // else
    // {
    //     printk("No network interface found!\n");
    // }

    return 0;
}
