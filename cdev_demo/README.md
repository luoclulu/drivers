一、字符设备结构体

二、字符设备API

三、字符设备的驱动架构
（1）
（2）
（3）
（4）
模板程序
```c
/*
设备结构体
*/
struct xxx_dev_t{
    struct cdev cdev;
    ...
};

struct xxx_dev_t *dev;
dev_t devno;

//读设备
ssize_t xxx_read(struct file *filp, char __user *buf, size_t count, loff_t* f_pos)
{
    ...
    copy_to_user(buf, ..., ...);
}
//写设备
ssize_t xxx_write(struct file *filp, const char __user *buf, size_t count, loff_t* f_pos)
{
    ...
    copy_from_user(..., buf, ...);
}

//操作函数file_operations
struct file_operations xxx_fops = {
    .owner = THIS_MODULE,
    .read = xxx_read,
    .write = xxx_write,
    ...
};

//设备驱动模块加载函数
static int __init xxx_init(void)
{
    ...
    devno = MKDEV(xxx_major, 0);
    //(1)申请设备号
    if(xxx_major)
    {
        register_chrdev_region(devno, 1, "xxx_dev");
    } 
    else
    {
        alloc_chrdev_region(&devno, 0, 1, "xxx_dev");
    }
    //(2)为设备结构体申请内存(推荐使用devm_kzalloc)
    dev = kzalloc(sizeof(struct xxx_dev_t), GFP_KERNEL); 
    //(3)初始化cdev
    cdev_init(&dev.cdev, &xxx_fops);
    dev.cdev.owner = THIS_MODULE;
    //(4)向系统注册设备
    cdev_add(dev.cdev, dev_no, 1);
}
module_init(xxx_init);

//设备驱动模块卸载函数
static void __exit xxx_exit(void)
{
    //释放设备号
    unregister_chrdev_region(dev_no, 1);
    //注销设备
    cdev_del(&dev.cdev);
    ...
}
module_exit(xxx_exit);
MODULE_LICENSE("GPL v2");
```
四、简化字符设备驱动架构

五、自动创建设备API

```c
//创建一个类,在sys/class/目录下创建一个类
#define class_create(owner, name)		\
({						\
	static struct lock_class_key __key;	\
	__class_create(owner, name, &__key);	\
})

//创建一个设备(在/dev目录下创建设备文件)，并注册到sysfs
struct device *device_create(struct class *class, struct device *parent,
			     dev_t devt, void *drvdata, const char *fmt, ...)
```



六、地址映射API

```CQL
void __iomem *ioremap(resource_size_t res_cookie, size_t size)
```

