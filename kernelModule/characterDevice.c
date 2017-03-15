   
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
	
  
     
    #define DEVICE_NAME "characterDevice"
 
    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("Magnus Östgren and Magnus Sörensen");
    MODULE_DESCRIPTION("A simple character device driver to comunicate with an characterDevice micro controller");
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
            printk(KERN_ALERT "characterDevice: cannot obtain major number %d.\n", device_major);
            return ret;
        }

        printk(KERN_INFO "characterDevice: chrdev loaded.\n");
        return 0;
    }
     
    void device_exit() {
        unregister_chrdev(device_major, DEVICE_NAME);
        printk(KERN_INFO "characterDevice: chrdev unloaded.\n");
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
  size_t dataLength;
  int clientData = 0;
    static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *offset) {
        char dataBuffer[length];
        memset(dataBuffer, 0, length);
        snprintf(dataBuffer, length, "%d\n", clientData);
        printk(KERN_INFO "characterDevice:%s", dataBuffer); 
        for(dataLength = 0; dataBuffer[dataLength] != 0; dataLength++);
        copy_to_user(buff, dataBuffer, dataLength);
        return dataLength;
    }
 
    static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *offset) {

        if(0 == kstrtoint_from_user(buff, length, 10, &clientData)){
            printk(KERN_INFO "characterDevice:got %d that squared is %d\n", clientData, clientData * clientData);
        }else{
            printk(KERN_INFO "characterDevice:Error, non integer input");
        }

        return length;
    }
