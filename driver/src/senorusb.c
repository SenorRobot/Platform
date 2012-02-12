#include <linux/module.h>
#include "senorusb.h"

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(USB_ID_VENDOR, USB_ID_PRODUCT) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

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
	printk("usb_probe()\n");
	printk(" idVendor:  %04X\n", interface_to_usbdev(intf)->descriptor.idVendor);
	printk(" idProduct: %04X\n", interface_to_usbdev(intf)->descriptor.idProduct);
	printk(" minor:     %04X\n", intf->minor);
	if (interface_to_usbdev(intf)->descriptor.idVendor == USB_ID_VENDOR &&
	    interface_to_usbdev(intf)->descriptor.idProduct == USB_ID_PRODUCT)
	{
		try_module_get(THIS_MODULE);
		printk("CONNECTED\n");

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

	printk("DISCONNECTED\n");
	module_put(THIS_MODULE);
}

/*int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf) {

}*/

