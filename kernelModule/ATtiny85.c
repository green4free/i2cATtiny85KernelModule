   
    #include <linux/kernel.h>
    #include <linux/module.h>
    #include <linux/moduleparam.h>
    #include <linux/init.h>
    #include <linux/slab.h>
    #include <linux/fs.h>
    #include <linux/fcntl.h>
    #include <linux/stat.h>
    #include <linux/types.h>
    #include <linux/errno.h>
    #include <asm/uaccess.h>
	
	#include <linux/i2c.h>

     
    #define DEVICE_NAME "ATtiny85"
     
    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("Magnus Östgren and Magnus Sörensen");
    MODULE_DESCRIPTION("A simple character device driver to comunicate with an ATtiny micro controller");
    MODULE_SUPPORTED_DEVICE(DEVICE_NAME);
     
    int device_init(void);
    void device_exit(void);
    static int device_open(struct inode *, struct file *);
    static int device_release(struct inode *, struct file *);
    static ssize_t device_read(struct file *, char *, size_t, loff_t *);
    static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
     
    module_init(device_init);
    module_exit(device_exit);
     
    static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
    };
     
    static int device_major = 60;
    static int device_opend = 0;
     
    module_param(device_major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    MODULE_PARM_DESC(device_major, DEVICE_NAME " major number");
     
    int device_init() {
        int ret;
        ret = register_chrdev(device_major, DEVICE_NAME, &fops);
        if(ret < 0) {
            printk(KERN_ALERT "ATtiny85: cannot obtain major number %d.\n", device_major);
            return ret;
        }
        memset(device_buffer, 0, BUFFER_SIZE);
        printk(KERN_INFO "ATtiny85: chrdev loaded.\n");
        return 0;
    }
     
    void device_exit() {
        unregister_chrdev(device_major, DEVICE_NAME);
        printk(KERN_INFO "ATtiny85: chrdev unloaded.\n");
    }
     
    static int device_open(struct inode *nd, struct file *fp) {
        if(device_opend) return -EBUSY;
        device_opend++;
        try_module_get(THIS_MODULE);
        return 0;
    }
     
    static int device_release(struct inode *nd, struct file *fp) {
        if(device_opend) device_opend--;
        module_put(THIS_MODULE);
        return 0;
    }
     
    static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *offset) {
        int clientData = i2c_smbus_read_byte_data(0x04, 0x04);
        snprintf(buff, 4, "%d", clientData);
        return clientData;
    }
     
    static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *offset) {
        int clientData;
        kstrtoint_from_user(buff, length, 10, &clientData);
        return i2c_smbus_write_byte_data(0x04, 0x04, clientData);
    }
     
