#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>


static struct resource led_resource[] = {
    [0] = {
        .start = 0x56000050,
        .end   = 0x56000050 + 8 - 1,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 5,
        .end   = 5,
        .flags = IORESOURCE_IRQ,
    }

};

static void led_release(struct device * dev)
{

}


static struct platform_device led_dev = {
    .name         = "my_led",
    .id       = -1,
    .num_resources    = ARRAY_SIZE(led_resource),
    .resource     = led_resource,
    .dev = { 
    	.release = led_release, 
	},
};

static int platform_led_device_init(void)
{
	platform_device_register(&led_dev);
	return 0;
}

static void platform_led_device_exit(void)
{
	platform_device_unregister(&led_dev);
}

module_init(platform_led_device_init);
module_exit(platform_led_device_exit);

MODULE_LICENSE("GPL");

