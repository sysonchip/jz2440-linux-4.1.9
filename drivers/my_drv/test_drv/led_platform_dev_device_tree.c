/*
 * @Description: 基于平台设备模型的device.c
 */
#include <linux/init.h>  
#include <linux/module.h> 
#include <linux/platform_device.h> 


void led_release(struct device *dev)
{

}

struct resource led_res[] = {
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

struct platform_device led_device = {
	.name = "guan_led",
	.id = -1,
	.resource = led_res,
	.num_resources = ARRAY_SIZE(led_res),
	.dev = {
		.release = led_release}
};

static int led_device_init(void)
{
	platform_device_register(&led_device);
	return 0;
}

static void led_device_exit(void)
{
	platform_device_unregister(&led_device);
}

module_init(led_device_init);
module_exit(led_device_exit);
MODULE_LICENSE("GPL");