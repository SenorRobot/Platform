#ifndef _SENORUSB_H_
#define _SENORUSB_H_

#include <linux/usb.h>

#define USB_CTRL_INTERFACE_ID 0x0000
#define USB_DATA_INTERFACE_ID 0x0001
#define USB_ID_VENDOR    0x03EB
#define USB_ID_PRODUCT   0x0506

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Crawford");

struct platform {
	uint8_t motor_l_out;
	uint8_t motor_r_out;
};

int usb_probe(struct usb_interface *intf, const struct usb_device_id *id_table);
void usb_disconnect(struct usb_interface *intf);
//int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf);
size_t motor_store(struct device *dev, struct device_attribute *attr, struct platform *robot);

#endif

