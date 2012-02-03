#include <usb.h>

int initialize_driver() {
	if (driver_info == NULL) {
		driver_info = malloc(sizeof(struct usb_driver));
		if (driver_info == NULL) return -1;

		driver_info->name = "SenorRobot";
		driver_info->probe = &usb_probe;
		driver_info->disconnect = &usb_disconnect;
		//driver_info->driver_list = ??;
		//driver_info->fops = ??;
		driver_info->minor = USB_MINOR;
		//driver_info->serialize = ??;
		driver_info->usb_ioctl;
		//driver_info->id_table = ??;
	}

	return usb_register(driver_info);
}

void *usb_probe(struct usb_device *dev, unsigned int interface, const struct usb_device_id *id_table) {
	struct driver_context *context = NULL;

	if (dev->descriptor.idVendor == USB_ID_VENDOR &&
	    dev->descriptor.idProduct == USB_ID_PRODUCT &&
	    interface == USB_INTERFACE_ID) {
		MOD_INC_USE_COUNT;

		context = allocate_driver_resources();
	}

	return context;
}

void usb_disconnect(struct usb_device *dev, void *context) {
	context->remove_pending = 1;
	wake_up(&context->wait);
	sleep_on(&context->remove_ok);
	free_driver_resources(context);

	MOD_DEC_USE_COUNT;
}

int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf) {

}

