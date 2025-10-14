#include <zephyr/kernel.h>

int main(void)
{
	int count = 0;

	while (1) {
		
		printk("counter: %d\n", count++);

		k_sleep(K_MSEC(1000));
	}

	return 0;
}

