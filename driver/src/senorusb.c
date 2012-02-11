#include <linux/module.h>
#include "senorusb.h"

static struct usb_driver driver_info = {
	.name = "SenorRobot",
	.probe = &usb_probe
	//.disconnect = &usb_disconnect,
	//.owner = THIS_MODULE,
	//.driver_list = ??;
	//.fops = ??;
	//.minor = USB_MINOR,
	//.serialize = ??;
	//.usb_ioctl;
	//.id_table = static struct usb_device_id [] {
	//	{ USB_DEVICE(USB_ID_VENDOR, USB_ID_PRODUCT); },
	//	{ },
	//}
};

int __init init_module() {
	int retval = usb_register(&driver_info);
	if (retval) {
		err("usb_register(): error %d\n", retval);
		return -1;
	}

	return 0;
}

void __exit cleanup_module() {
	usb_deregister(&driver_info);
}

//void *usb_probe(struct usb_device *dev, unsigned int interface, const struct usb_device_id *id_table) {
int usb_probe(struct usb_interface *intf, const struct usb_device_id *id_table) {
	if (interface_to_usbdev(intf)->descriptor.idVendor == USB_ID_VENDOR &&
	    interface_to_usbdev(intf)->descriptor.idProduct == USB_ID_PRODUCT &&
	    intf->minor == USB_INTERFACE_ID)
	{
		try_module_get(THIS_MODULE);
		printk("CREATED DRIVER\n");
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

