CC      = avr-gcc
OBJCOPY = avr-objcopy

MCU = atmega32u4

AVRDUDE            = avrdude
AVRDUDE_PORT       = /dev/teensy
AVRDUDE_MCU        = $(MCU)
AVRDUDE_PROGRAMMER = avr109

CFLAGS  = -mmcu=$(MCU)
CFLAGS += -I.
CFLAGS += -funsigned-char
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -std=c99


TARGET    = platform
BUILD_DIR = build
SRC_DIR   = src

HEX = $(BUILD_DIR)/$(TARGET).hex
ELF = $(BUILD_DIR)/$(TARGET).elf

SRC  = $(TARGET).c    \
      descriptors.c

OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)


$(BUILD_DIR)/$(TARGET).hex: $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock $< $@

.SECONDARY: $(ELF)
$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $^ --output $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.hex
	rm -f $(BUILD_DIR)/*.elf

burn: $(HEX)
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -U flash:w:$(HEX)
