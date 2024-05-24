#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <asm/io.h>

static int major;


static struct class *cls;
static volatile unsigned long *gpio_con;
static volatile unsigned long *gpio_dat;
static int pin;

static int led_open(struct inode *inode, struct file *file)
{
	printk("---------------led_open\n");
	//*gpio_con &= ~(0x3<<(pin*2));
	//*gpio_con |= (0x1<<(pin*2));

	*gpio_con &= ~((0x3<<(4*2)) | (0x3<<(5*2)) | (0x3<<(6*2)));
	*gpio_con |= ((0x1<<(4*2)) | (0x1<<(5*2)) | (0x1<<(6*2)));
	return 0;	
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;

	//printk("first_drv_write\n");

	copy_from_user(&val, buf, count); //	copy_to_user();

	if (val == 1)
	{
		// ���
		//*gpio_dat &= ~(1<<pin);
		*gpio_dat &= ~((1<<4) | (1<<5) | (1<<6));
	}
	else
	{
		// ���
		*gpio_dat |= (1<<4) | (1<<5) | (1<<6);

	}
	
	return 0;
}

static const struct of_device_id of_match_leds[] = {
	{ .compatible = "jz2440_led", .data = NULL },
	{ /* sentinel */ }
};

static struct file_operations led_fops = {
    .owner  =   THIS_MODULE,   
    .open   =   led_open,     
	.write	=	led_write,	   
};

static int platform_led_driver_probe(struct platform_device *pdev)
{
	struct resource		*res;


	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpio_con = ioremap(res->start, res->end - res->start + 1); 
	gpio_dat = gpio_con + 1;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	pin = res->start;

	printk("---------------------platform_led_driver_probe, found led\n");

	major = register_chrdev(0, "my_led", &led_fops);

	cls = class_create(THIS_MODULE, "my_led");

	device_create(cls, NULL, MKDEV(major, 0), NULL, "led"); /* /dev/led */
	
	return 0;
}

static int platform_led_driver_remove(struct platform_device *pdev)
{
	/* iounmap */
	printk("platform_led_driver_remove, remove led\n");

	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, "my_led");
	iounmap(gpio_con);
	
	return 0;
}


struct platform_driver led_drv = {
	.probe		= platform_led_driver_probe,
	.remove		= platform_led_driver_remove,
	.driver		= {
		.name	= "my_led",
		//.of_match_table = of_match_leds, 
	}
};


static int platform_led_driver_init(void)
{
	platform_driver_register(&led_drv);

	printk("------------00---------platform_led_driver_init\n");
	return 0;
}

static void platform_led_driver_exit(void)
{
	platform_driver_unregister(&led_drv);
}

module_init(platform_led_driver_init);
module_exit(platform_led_driver_exit);

MODULE_LICENSE("GPL");



