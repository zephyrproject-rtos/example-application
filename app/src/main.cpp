#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <iostream>
#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);


int main()
{
    /* Main Function */
    std::cout<<"Hello World!\n";

    while(1)
    {
        /* Infinite Loop */
        k_msleep(1000);
    }
    return 0;
}