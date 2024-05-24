#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
//#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/regs-gpio.h> 
#include <mach/hardware.h>
//#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/device.h>

static int major;

static int platform_led_open(struct inode *inode, struct file *file) 
{
	printk("platform_led_open\n");
	return 0;
}

static int platform_led2_open(struct inode *inode, struct file *file)
{
	printk("platform_led2_open\n");
	return 0;
}


static struct file_operations platform_led_fops = {
	.owner = THIS_MODULE,
	.open  = platform_led_open,
};

static struct file_operations platform_led2_fops = {
	.owner = THIS_MODULE,
	.open  = platform_led2_open,
};


#define MAJOR_COUNT   2

static struct cdev platform_led_cdev;
static struct cdev platform_led2_cdev;
static struct class *cls;

static int platform_led_init(void)
{
	dev_t devid;
	
	if (major) {
		devid = MKDEV(major, 0);
		register_chrdev_region(devid, MAJOR_COUNT, "platform_led");  /* (major,0~1) ��Ӧ platform_led_fops, (major, 2~255)������Ӧplatform_led_fops */
	} else {
		alloc_chrdev_region(&devid, 0, MAJOR_COUNT, "platform_led"); /* (major,0~1) ��Ӧ platform_led_fops, (major, 2~255)������Ӧplatform_led_fops */
		major = MAJOR(devid);                     
	}
	
	cdev_init(&platform_led_cdev, &platform_led_fops);
	cdev_add(&platform_led_cdev, devid, MAJOR_COUNT);

	devid = MKDEV(major, 2);
	register_chrdev_region(devid, 1, "platform_led2");
	cdev_init(&platform_led2_cdev, &platform_led2_fops);
	cdev_add(&platform_led2_cdev, devid, 1);
	

	cls = class_create(THIS_MODULE, "platform_led");
	device_create(cls, NULL, MKDEV(major, 0), NULL, "platform_led0"); /* /dev/platform_led0 */
	device_create(cls, NULL, MKDEV(major, 1), NULL, "platform_led1"); /* /dev/platform_led1 */
	device_create(cls, NULL, MKDEV(major, 2), NULL, "platform_led2"); /* /dev/platform_led2 */
	device_create(cls, NULL, MKDEV(major, 3), NULL, "platform_led3"); /* /dev/platform_led3 */
	
	printk("----------------platform_led_init--------\n");
	return 0;
}

static void platform_led_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	device_destroy(cls, MKDEV(major, 1));
	device_destroy(cls, MKDEV(major, 2));
	device_destroy(cls, MKDEV(major, 3));
	class_destroy(cls);

	cdev_del(&platform_led_cdev);
	unregister_chrdev_region(MKDEV(major, 0), MAJOR_COUNT);

	cdev_del(&platform_led2_cdev);
	unregister_chrdev_region(MKDEV(major, 2), 1);
}

module_init(platform_led_init);
module_exit(platform_led_exit);


MODULE_LICENSE("GPL");

