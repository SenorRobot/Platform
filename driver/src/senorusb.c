#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include "senorusb.h"

#define MOTOR_SHOW_STORE(val)                                                                                      \
ssize_t motor_store_ ## val (struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {   \
	struct usb_interface *intf = to_usb_interface(dev);                                                            \
	struct platform *robot = usb_get_intfdata(intf);                                                               \
	robot->motor_ ## val ## _out = *buf;                                                                           \
	motor_store(dev, attr, robot);                                                                                 \
	return 1;                                                                                                      \
}                                                                                                                  \
ssize_t motor_show_ ## val (struct device *dev, struct device_attribute *attr, char *buf) {                        \
	struct usb_interface *intf = to_usb_interface(dev);                                                            \
	struct platform *robot = usb_get_intfdata(intf);                                                               \
	return scnprintf(buf, PAGE_SIZE, "%d", robot->motor_ ## val ## _out);                                          \
}


MOTOR_SHOW_STORE(l);
MOTOR_SHOW_STORE(r);

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(USB_ID_VENDOR, USB_ID_PRODUCT) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

DEVICE_ATTR(motor_l, 0x0644, motor_show_l, motor_store_l);
DEVICE_ATTR(motor_r, 0x0644, motor_show_r, motor_store_r);

static struct usb_driver driver_info = {
	.name = "SenorRobot",
	.probe = &usb_probe,
	.id_table = id_table,
	.disconnect = &usb_disconnect
	//.owner = THIS_MODULE,
	//.driver_list = ??;
	//.fops = ??;
	//.minor = USB_MINOR
	//.serialize = ??;
	//.usb_ioctl;
};

int __init init_module() {
	int retval = usb_register(&driver_info);
	if (retval) {
		err("usb_register(): error %d\n", retval);
		return -1;
	}

	printk("REGISTERED DRIVER\n");
	return 0;
}

void __exit cleanup_module() {
	printk("DESTROYING DRIVER\n");
	usb_deregister(&driver_info);
}

//void *usb_probe(struct usb_device *dev, unsigned int interface, const struct usb_device_id *id_table) {
int usb_probe(struct usb_interface *intf, const struct usb_device_id *id_table) {
	int ret;
	struct platform *robot;

	printk("usb_probe()\n");
	printk(" idVendor:  %04X\n", interface_to_usbdev(intf)->descriptor.idVendor);
	printk(" idProduct: %04X\n", interface_to_usbdev(intf)->descriptor.idProduct);
	printk(" minor:     %04X\n", intf->minor);
	printk(" interface: %04X\n", intf->cur_altsetting->desc.bInterfaceNumber);
	if (interface_to_usbdev(intf)->descriptor.idVendor == USB_ID_VENDOR &&
	    interface_to_usbdev(intf)->descriptor.idProduct == USB_ID_PRODUCT &&
	    intf->cur_altsetting->desc.bInterfaceNumber == USB_INTERFACE_ID)
	{
		try_module_get(THIS_MODULE);
		printk("CONNECTED\n");

		if (ret = device_create_file(&intf->dev, &dev_attr_motor_l), ret) {
			printk("device_create_file(): %d\n", ret);
		}
		if (ret = device_create_file(&intf->dev, &dev_attr_motor_r), ret) {
			printk("device_create_file(): %d\n", ret);
		}

		robot = kmalloc(sizeof(struct platform), GFP_KERNEL);
		robot->motor_l_out = 0;
		robot->motor_r_out = 0;
		usb_set_intfdata(intf, robot);

		return 0;
	}
	printk("NOT RECOGNIZED\n");

	return -ENODEV;
}

void usb_disconnect(struct usb_interface *intf) {
/*	context->remove_pending = 1;
	wake_up(&context->wait);
	sleep_on(&context->remove_ok);
	free_driver_resources(context);*/

	device_remove_file(&intf->dev, &dev_attr_motor_l);
	device_remove_file(&intf->dev, &dev_attr_motor_r);

	kfree(usb_get_intfdata(intf));

	printk("DISCONNECTED\n");
	module_put(THIS_MODULE);
}


size_t motor_store(struct device *dev, struct device_attribute *attr, struct platform *robot) {
	struct urb request;
	int result;

	struct usb_interface *intf = to_usb_interface(dev);
	struct usb_device *usbdev = interface_to_usbdev(intf);
	char buf[2] = { robot->motor_l_out, robot->motor_r_out };
	usb_fill_bulk_urb(&request, usbdev, usb_sndbulkpipe(usbdev, 1), buf, 2, NULL, NULL);

	result = usb_submit_urb(&request, 0);
	if (result) {
		// Doesn't show up
		printk("Error writing urb (%d)", result);
		return -1;
	} else {
		printk("Wrote 0x%02X%02X to device", buf[0], buf[1]);
	}
	return 2;
}

/*int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf) {

}*/

