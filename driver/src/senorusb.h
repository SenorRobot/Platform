#ifndef _SENORUSB_H_
#define _SENORUSB_H_

#define USB_MINOR        1
#define USB_INTERFACE_ID 1
#define USB_ID_VENDOR    0x1234
#define USB_ID_PRODUCT   0x5678

struct usb_driver *driver_info = NULL;

void *usb_probe(struct usb_device *dev, unsigned int interface, const struct usb_device_id *id_table);
void usb_disconnect(struct usb_device *dev, void *context);
int usb_ioctl(struct usb_device *dev, unsigned int code, void *buf);

#endif

