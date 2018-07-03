TOOLCHAIN=xtensa-lx106-elf-
OBJCOPY = xtensa-lx106-elf-objcopy

CCFLAGS += 			\
	-nostdlib	\
	-g			\
	-Wpointer-arith		\
	-Wundef			\
	-Werror			\
	-Wl,-EL			\
	-fno-inline-functions	\
	-nostdlib       \
	-mlongcalls	\
	-mtext-section-literals \
	-ffunction-sections \
	-fdata-sections	\
	-fno-builtin-printf

TARGET_LDFLAGS =  -Wl,--no-check-sections -Wl,--gc-sections -u call_user_start -Wl,-static -Wl,--start-group
LIBS_PATH = -L$(ESP_SDK)/lib
LIB_LD = -T$(ESP_SDK)/ld/eagle.app.v6.ld
LIBS = -lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lmain 	-lespnow -lcrypto 

TARGET_LDFLAGS += -g -O2    
INCLUDES +=	-I$(ESP_SDK)/include -I$(ESP_SDK)/driver_lib/include -I.	

BINDIR=../../bin
OUTBIN=../../out

TARGETLIB = $(OUTBIN)/libuser.a
TARGET = $(OUTBIN)/user_image.o
IMAGE = $(OUTBIN)/blink.bin

#//xtensa-lx106-elf-gcc  -L../lib -nostdlib -T../ld/eagle.app.v6.ld 
#-Wl,--no-check-sections -Wl,--gc-sections -u call_user_start -Wl,-static -Wl,--start-group 
#-lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lmain -lespnow -lcrypto 
#user/.output/eagle/debug/lib/libuser.a -Wl,--end-group -o .output/eagle/debug/image/eagle.app.v6.out 

.PHONY: default all clean

default: $(IMAGE)

all: default
		
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(TOOLCHAIN)gcc $(CCFLAGS) $(INCLUDES) -c $< -o $@

.PRECIOUS: $(TARGETLIB) $(OBJECTS)

$(TARGETLIB): $(OBJECTS)
	$(TOOLCHAIN)ar ru $@ $^

$(TARGET): $(TARGETLIB)
	$(TOOLCHAIN)gcc $(LIBS_PATH) -nostdlib $(LIB_LD) $(TARGET_LDFLAGS) $(LIBS) $< -Wl,--end-group -o $@

$(IMAGE): $(TARGET)
	@$(OBJCOPY) --only-section .text -O binary $< eagle.app.v6.text.bin
	@$(OBJCOPY) --only-section .data -O binary $< eagle.app.v6.data.bin
	@$(OBJCOPY) --only-section .rodata -O binary $< eagle.app.v6.rodata.bin
	@$(OBJCOPY) --only-section .irom0.text -O binary $< eagle.app.v6.irom0text.bin
	@python $(ESP_SDK)/tools/gen_appbin.py $(TARGET) 0 2 0 6 0
	
clean:
	-rm -f *.o
	-rm -f $(TARGET)
