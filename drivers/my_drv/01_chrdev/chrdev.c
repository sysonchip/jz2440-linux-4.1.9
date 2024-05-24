/*
 * @Description:字符设备自动创建设备节点步骤一创建类，创建设备
 */
#include <linux/init.h>	  //初始化头文件
#include <linux/module.h> //最基本的文件，支持动态添加和卸载模块。
#include <linux/fs.h>	  //包含了文件操作相关struct的定义，例如大名鼎鼎的struct file_operations
#include <linux/kdev_t.h>
#include <linux/cdev.h>		   //对字符设备结构cdev以及一系列的操作函数的定义。//包含了cdev 结构及相关函数的定义。

#include <linux/device.h>	   //包含了device、class 等结构的定义



#define DEVICE_NUMBER 1		   //定义次设备号的个数
#define DEVICE_SNAME "schrdev" //定义静态注册设备的名称
#define DEVICE_ANAME "achrdev" //定义动态注册设备的名称
#define DEVICE_MINOR_NUMBER 0  //定义次设备号的起始地址

#define DEVICE_CLASS_NAME "chrdev_class"
#define DEVICE_NODE_NAME "chrdev_test" //宏定义设备节点的名字
static int major_num, minor_num;	   //定义主设备号和次设备号

struct class *class;				   /* 类 */
struct device *device;				   /* 设备 */
struct cdev cdev;					   //定义一个cdev结构体
module_param(major_num, int, S_IRUSR); //驱动模块传入普通参数major_num
module_param(minor_num, int, S_IRUSR); //驱动模块传入普通参数minor_num
dev_t dev_num;						   /* 设备号 */


int chrdev_open(struct inode *inode, struct file *file)
{
	printk("chrdev_open\n");
	return 0;
}

struct file_operations chrdev_ops = {
	.owner = THIS_MODULE,
	.open = chrdev_open};

static int chrdev_init(void)
{
	int ret; 
	if (major_num)
	{
		/*静态注册设备号*/
		printk("major_num = %d\n", major_num); //打印传入进来的主设备号
		printk("minor_num = %d\n", minor_num); //打印传入进来的次设备号

		dev_num = MKDEV(major_num, minor_num);								//MKDEV将主设备号和次设备号合并为一个设备号
		ret = register_chrdev_region(dev_num, DEVICE_NUMBER, DEVICE_SNAME); //注册设备号
		if (ret < 0)
		{
			printk("register_chrdev_region error\n");
		}
		printk("---------------register_chrdev_region ok\n"); //静态注册设备号成功
	}
	else
	{
		/*动态注册设备号*/
		ret = alloc_chrdev_region(&dev_num, DEVICE_MINOR_NUMBER, 1, DEVICE_ANAME);
		if (ret < 0)
		{
			printk("alloc_chrdev_region error\n");
		}
		printk("------------alloc_chrdev_region ok\n"); //动态注册设备号成功

		major_num = MAJOR(dev_num);			   //将主设备号取出来
		minor_num = MINOR(dev_num);			   //将次设备号取出来
		printk("major_num = %d\n", major_num); //打印传入进来的主设备号
		printk("minor_num = %d\n", minor_num); //打印传入进来的次设备号
	}
	// 初始化 cdev
	cdev.owner = THIS_MODULE;
	cdev_init(&cdev, &chrdev_ops);
	// 向系统注册设备
	cdev_add(&cdev, dev_num, DEVICE_NUMBER);
	// 创建 class 类
	class = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
	// 在 class 类下创建设备
	device = device_create(class, NULL, dev_num, NULL, DEVICE_NODE_NAME);
	return 0;
}


static void chrdev_exit(void)
{
	//注销设备号
	unregister_chrdev_region(MKDEV(major_num, minor_num), DEVICE_NUMBER);
	//删除设备
	cdev_del(&cdev);
	//注销设备
	device_destroy(class, dev_num);
	//删除类
	class_destroy(class);
	printk("gooodbye! \n");
}
// 将上面两个函数指定为驱动的入口和出口函数
module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("guan");