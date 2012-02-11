#ifndef _SENORUSB_H_
#define _SENORUSB_H_

#include <linux/usb.h>

#define USB_MINOR        1
#define USB_INTERFACE_ID 1
#define USB_ID_VENDOR    0x03EB
#define USB_ID_PRODUCT   0x0506

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Crawford");

int usb_probe(struct usb_interface *intf, const struct usb_device_id *id_table);
void usb_disconnect(struct usb_interface *intf);
//int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf);

#endif

