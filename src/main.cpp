#include <zephyr/kernel.h>

#define SLEEP_TIME_MS 250

int main(void)
{
	while (1) {
		printk("Hello, World!\n");
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
