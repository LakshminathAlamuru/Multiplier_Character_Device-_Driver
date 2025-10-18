# include <linux/module.h> /* Needed by all modules */
# include <linux/kernel.h> /* Needed for KERN * and printk */
# include <linux/fs .h> /* Needed for file o per at io ns */
# include <linux/cdev.h> /* Needed for char device drivers */
# include <asm/uaccess.h> /* Needed for copy_to_user and copy_from_user */
# include <asm/io.h> /* Needed for IO reads and writes */
# include <linux/types.h> /* Needed for dev_t type definition */
# include <stddef.h> /* Include the header file for NULL definition */
# include <linux/init.h> /* Needed for the macros */
# include "xparameters.h" /* needed for physical address of multiplier */
/* Define physical address XPAR_MULTIPLY_0_S00_AXI_BASEADDR*/
# define PHY_ADDR XPAR_MULTIPLY_0_S00_AXI_BASEADDR
/* Define size of physical address range for multiply */
# define MEMSIZE (XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR + 1)

/* Device name */
#define DeviceName "multiplier"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>

struct cdev *mychardev;
int ret;
int Major;
dev_t dev_num;
void *virtaddr; /* virtual address pointing to multiplier */
int majorNumber;

/* Function prototypes */
int openDevice(struct inode *inode, struct file *filp);
int releaseDevice(struct inode *inode, struct file *filp);
ssize_t readResult(struct file *filp, char *buffer, size_t length, loff_t *offset);
ssize_t writeValue(struct file *filp, const char *buff, size_t len, loff_t *off);

/* File operations structure */
struct file_operations fops = {
    .owner = THIS_MODULE,
    .llseek = NULL,
    .read = readResult,
    .write = writeValue,
    .open = openDevice,
    .release = releaseDevice,
};

/*
 * Called when a process tries to open the device file
 */
int openDevice(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "MULTIPLY: Opening device\n");
    return 0;
}

/*
 * Called when a process tries to close the device file
 */
int releaseDevice(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "MULTIPLY: Closing device\n");
    return 0;
}

/*
 * Called when a process reads from the device file
 */
ssize_t readResult(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;
    int i = 0;
    int result1 = ioread32(virtaddr + 0);
    int result2 = ioread32(virtaddr + 4);
    int result3 = ioread32(virtaddr + 8);
    char finalmsg[12];

    printk(KERN_INFO "result1 = %d, result2 = %d, result3 = %d\n", result1, result2, result3);

    memcpy(finalmsg, &result1, 4);
    memcpy(&finalmsg[4], &result2, 4);
    memcpy(&finalmsg[8], &result3, 4);

    while (length && i < sizeof(finalmsg)) {
        put_user(finalmsg[i], buffer++);
        i++;
        length--;
        bytes_read++;
    }

    return bytes_read;
}

/*
 * Called when a process writes to the device file
 */
ssize_t writeValue(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    ssize_t i;
    char inputmsg[8];
    int numbers[2];

    printk(KERN_INFO "MULTIPLY: DEVICE write\n");

    /* get_user pulls message from userspace into kernel space */
    for (i = 0; i < 8; i++)
        get_user(inputmsg[i], buff + i);

    memcpy(numbers, inputmsg, 8);

    /* Write input1 to register 0 */
    printk(KERN_INFO "Writing inp1 to register 0\n");
    iowrite32(numbers[0], virtaddr + 0);

    /* Write input2 to register 1 */
    printk(KERN_INFO "Writing inp2 to register 1\n");
    iowrite32(numbers[1], virtaddr + 4);

    return i;
}

int my_init(void)
{
    /* map virtual address to multiplier physical address*/
    virtaddr = ioremap(PHY_ADDR, MEMSIZE);
    // register the device
    /* The function call registers a device and returns a major number associated with it. Be wary, the device file could be accessed as soon as you register with it
    ,make sure anything you need(ie buffers etc) are setup _BEFORE_ you register the device. */
    Major = register_chrdev(0, DeviceName, &fops);

    /*Negative values indicates a problem*/
    if(Major < 0)
    {
        printl(KERN_ALERT, "Registering char device failed with %d\n", Major);
        return Major;
    }
}


void my_cleanup(void)
{
/* Unregister the character device */
unregister_chrdev(Major , DeviceName);
iounmap((void*)virtaddr);
}

module_init(my_init);
module_exit(my_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshminath Reddy Alamuru");
MODULE_DESCRIPTION("Multiplier Character Device Driver");

