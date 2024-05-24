/*
 * @Description: 设备树下的平台总线驱动，匹配成功后，去设备树文件中获取硬件信息，然后物理地址映射为虚拟地址，接下来可以注册字符设备和杂项设备
 
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>

int size;
int ret = 0;
u32 out_values[2] = {0};
const char *str;

struct device_node *test_device_node;
struct property *test_node_property;
unsigned int *vir_gpio_dr;
int led_probe(struct platform_device *pdev)
{ 
	printk("------------------led_probe\n");
	/*********************方法一：直接获取节点**************************/
	//printk("node name is %s\n",pdev->dev.of_node->name);

	/*********************方法二：通过函数获取硬件资源**************************/
	 test_device_node = of_find_node_by_path("/test");  //获得设备节点
	if(test_device_node == NULL){
		printk("of_find_node_by_path is error \n");
	return -1;
	}

#if 0 
	//将此段代码注释掉
	ret = of_property_read_u32_array(pdev->dev.of_node, "reg", out_values, 2);
	if (ret < 0)
	{
		printk("of_property_read_u32_array is error \n");
		return -1;
	}
	printk("out_values[0] is 0x%08x\n", out_values[0]);
	printk("out_values[1] is 0x%08x\n", out_values[1]);
	/*********************映射物理地址**************************/
	vir_gpio_dr = of_iomap(pdev->dev.of_node, 0);
	if (vir_gpio_dr == NULL)
	{
		printk("of_iomap is error \n");
		return -1;
	}
#endif
	printk("of_iomap is ok \n");
	return 0;
}
int led_remove(struct platform_device *pdev)
{
	printk("led_remove\n");
	return 0;
}




const struct platform_device_id led_idtable = {
	.name = "guan_led",
};

const struct of_device_id of_match_table_test[] = {
	{.compatible = "guan_led"},
	{},
};

struct platform_driver led_driver = {
	.probe = led_probe,
	.remove = led_remove, 
	.driver = {
		.owner = THIS_MODULE,
		.name = "guan_led000",
		//.of_match_table = of_match_table_test //接下来我们改一下驱动，让他来匹配设备树里面test的节点
	},
	.id_table = &led_idtable //.id_table的优先级要比driver.name的优先级要高，优先与.id_table进行匹配
};

static int led_driver_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&led_driver);
	if (ret < 0)
	{
		printk("platform_driver_register error \n");
	}
	printk("led platform_driver_register ok \n");

	return 0;
}

static void led_driver_exit(void)
{
	platform_driver_unregister(&led_driver);
}

module_init(led_driver_init);
module_exit(led_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("guan");
