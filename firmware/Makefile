CC      = avr-gcc
OBJCOPY = avr-objcopy

MCU = atmega32u4
F_CPU = 16000000
F_USB = $(F_CPU)

AVRDUDE            = avrdude
AVRDUDE_PORT       = /dev/teensy
AVRDUDE_MCU        = $(MCU)
AVRDUDE_PROGRAMMER = avr109

CFLAGS  = -mmcu=$(MCU)
CFLAGS += -I. -I$(LUFA_PATH)
CFLAGS += -funsigned-char
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -ffunction-sections
CFLAGS += -Wl,--gc-sections
CFLAGS += -Wall
CFLAGS += -std=c99
CFLAGS += -Os
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -DF_USB=$(F_USB)UL
CFLAGS += $(LUFA_OPTS)


TARGET    = platform
BUILD_DIR = build
SRC_DIR   = src

HEX = $(BUILD_DIR)/$(TARGET).hex
ELF = $(BUILD_DIR)/$(TARGET).elf

LUFA_OPTS  = -D USB_DEVICE_ONLY
LUFA_OPTS += -D DEVICE_STATE_AS_GPIOR=0
LUFA_OPTS += -D ORDERED_EP_CONFIG
LUFA_OPTS += -D FIXED_CONTROL_ENDPOINT_SIZE=8
LUFA_OPTS += -D FIXED_NUM_CONFIGURATIONS=1
LUFA_OPTS += -D USE_FLASH_DESCRIPTORS
LUFA_OPTS += -D USE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)"
LUFA_OPTS += -D BOARD=BOARD_ADAFRUITU4
LUFA_OPTS += -D ARCH=AVR8

LUFA_PATH = lufa-lib/trunk
include $(LUFA_PATH)/LUFA/makefile


SRC = $(SRC_DIR)/$(TARGET).c          \
      $(SRC_DIR)/descriptors.c        \
      $(SRC_DIR)/motor.c              \
      $(SRC_DIR)/pwm.c                \
      $(SRC_DIR)/gyro.c               \
      $(SRC_DIR)/spi.c                \
      $(SRC_DIR)/sonar.c              \
      $(LUFA_SRC_USB)                 \
      $(LUFA_SRC_USBCLASS)

OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)


all: $(HEX)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock $< $@

.SECONDARY: $(ELF)
$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $^ --output $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "`echo $@ | sed 's|[^/]*\.o||')`"
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(HEX)
	rm -f $(ELF)

burn: $(HEX)
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -U flash:w:$(HEX)
