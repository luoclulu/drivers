/*
�豸�ṹ��
*/
struct xxx_dev_t{
    struct cdev cdev;
    ...
};

struct xxx_dev_t *dev;
dev_t devno;

//���豸
ssize_t xxx_read(struct file *filp, char __user *buf, size_t count, loff_t* f_pos)
{
    ...
    copy_to_user(buf, ..., ...);
}
//д�豸
ssize_t xxx_write(struct file *filp, const char __user *buf, size_t count, loff_t* f_pos)
{
    ...
    copy_from_user(..., buf, ...);
}

//��������file_operations
struct file_operations xxx_fops = {
    .owner = THIS_MODULE,
    .read = xxx_read,
    .write = xxx_write,
    ...
};

//�豸����ģ����غ���
static int __init xxx_init(void)
{
    ...
    devno = MKDEV(xxx_major, 0);
    //(1)�����豸��
    if(xxx_major)
    {
        register_chrdev_region(devno, 1, "xxx_dev");
    } 
    else
    {
        alloc_chrdev_region(&devno, 0, 1, "xxx_dev");
    }
    //(2)Ϊ�豸�ṹ�������ڴ�(�Ƽ�ʹ��devm_kzalloc)
    dev = kzalloc(sizeof(struct xxx_dev_t), GFP_KERNEL); 
    //(3)��ʼ��cdev
    cdev_init(&dev.cdev, &xxx_fops);
    dev.cdev.owner = THIS_MODULE;
    //(4)��ϵͳע���豸
    cdev_add(dev.cdev, dev_no, 1);
}
module_init(xxx_init);

//�豸����ģ��ж�غ���
static void __exit xxx_exit(void)
{
    //�ͷ��豸��
    unregister_chrdev_region(dev_no, 1);
    //ע���豸
    cdev_del(&dev.cdev);
    ...
}
module_exit(xxx_exit);
MODULE_LICENSE("GPL v2");

