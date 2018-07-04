# Top level directory sturcture
TOPDIR= ../..
OBJDIR=$(TOPDIR)/out
BINDIR=$(TOPDIR)/bin

## App specyfic
TARGET_FW_IMAGE = $(BINDIR)/$(APP_NAME)
TARGET_APP_LIB = $(OBJDIR)/$(APP_NAME)_app.a
TARGET_LIB = $(BINDIR)/$(APP_NAME)_app.o

APP_HEADERS = $(wildcard *.h)
APP_SRC = $(wildcard *.c)

# Tool chain definitions
TOOLCHAIN=xtensa-lx106-elf-
OBJCOPY = xtensa-lx106-elf-objcopy

CCFLAGS = \
	-Os \
	-g \
	-Wpointer-arith \
	-Wundef \
	-Werror \
	-Wl,-EL \
	-fno-inline-functions \
	-nostdlib \
	-mlongcalls \
	-mtext-section-literals \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf  \
	-DICACHE_FLASH

LIBS_PATH = -L$(ESP_SDK)/lib
LIB_LD = -T$(ESP_SDK)/ld/eagle.app.v6.ld
LIBS = -lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lcrypto -lmain -ldriver
TARGET_LDFLAGS =  -Wl,--no-check-sections -Wl,--gc-sections -u call_user_start -Wl,-static -Wl,--start-group   
INCLUDES +=	-I$(ESP_SDK)/include -I$(ESP_SDK)/driver_lib/include	


APP_OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(APP_SRC))

$(TARGET_APP_LIB): $(APP_OBJECTS)
	$(TOOLCHAIN)ar ru $@ $^

$(TARGET_LIB): $(TARGET_APP_LIB)
	@mkdir -p $(@D)
	$(TOOLCHAIN)gcc $(LIBS_PATH) -nostdlib $(LIB_LD) $(TARGET_LDFLAGS) $(LIBS) $< -Wl,--end-group -o $@

$(TARGET_FW_IMAGE): $(TARGET_LIB)
	@$(OBJCOPY) --only-section .text -O binary $< eagle.app.v6.text.bin
	@$(OBJCOPY) --only-section .data -O binary $< eagle.app.v6.data.bin
	@$(OBJCOPY) --only-section .rodata -O binary $< eagle.app.v6.rodata.bin
	@$(OBJCOPY) --only-section .irom0.text -O binary $< eagle.app.v6.irom0text.bin
	@python $(ESP_SDK)/tools/gen_appbin.py  $(TARGET_LIB) 0 2 0 6 0	
	@rm eagle.app.v6.text.bin
	@rm eagle.app.v6.data.bin
	@rm eagle.app.v6.rodata.bin
	@mv eagle.app.v6.irom0text.bin $(TARGET_FW_IMAGE)_0x10000.bin
	@mv eagle.app.flash.bin $(TARGET_FW_IMAGE)_0.bin

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo $<
	@echo $@
	@echo $(APP_SRC)
	$(TOOLCHAIN)gcc $(CCFLAGS) $(INCLUDES) -c $< -o $@

.PRECIOUS: $(TARGET_IMAGE) $(TARGET_LIB) $(APP_OBJECTS)

.PHONY: default all clean flash


default: $(TARGET_FW_IMAGE)

all: default

clean:
	-rm -f $(OBJDIR)/*.o
	-rm -f $(OBJDIR)/*.a

debug:
	@echo $(TARGET_FW_IMAGE)
	@echo "debug"
	@echo $(APP_OBJECTS)
	@echo $(APP_SRC)
	@echo $(APP_HEADERS)

flash: $(TARGET_FW_IMAGE)	
	esptool.py write_flash  0x0 $(TARGET_FW_IMAGE)_0.bin 0x10000 $(TARGET_FW_IMAGE)_0x10000.bin --flash_mode dio --flash_size 32m
