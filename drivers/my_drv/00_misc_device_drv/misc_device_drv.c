/*
 * @Descripttion: 最简单的杂项设备驱动
 */
#include <linux/init.h>             //初始化头文件
#include <linux/module.h>           //最基本的文件，支持动态添加和卸载模块。
#include <linux/miscdevice.h>        /*注册杂项设备头文件*/
#include <linux/fs.h>                 /*注册设备节点的文件结构体*/
struct file_operations misc_fops={   //文件操作集
	.owner = THIS_MODULE
};
struct miscdevice  misc_dev = {     //杂项设备结构体
	.minor = MISC_DYNAMIC_MINOR,    //动态申请的次设备号
	.name = "hello_misc",		    //杂项设备名字是hello_misc
	.fops = &misc_fops,  			//文件操作集

};
static int misc_init(void){    //在初始化函数中注册杂项设备
	int ret;
	ret = misc_register(&misc_dev); 
	if(ret<0)
	{
		printk("misc registe is error \n");
	}
	printk("misc registe is succeed \n");
	return 0;
}
static void misc_exit(void) {   //在卸载函数中注销杂项设备
	misc_deregister(&misc_dev);
	printk(" misc gooodbye! \n");
}
module_init(misc_init);
module_exit(misc_exit);
MODULE_LICENSE("GPL");
