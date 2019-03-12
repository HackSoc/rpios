ARMGNU ?= arm-none-eabi
LINKER ?= build/kernel.ld

AOBJS := $(patsubst %.s,build/%.o,$(wildcard *.s))
COBJS := $(patsubst %.c,build/%.o,$(wildcard *.c))
OBJECTS := $(AOBJS) $(COBJS)
DEPS := $(COBJS:.o=.d)

CFLAGS += -std=c99 -mfloat-abi=hard -ffreestanding -fno-builtin -march=armv7-a -MD -MP -g

LDFLAGS += --no-undefined

TARGET = build/kernel.img

.PHONY: clean run all

all : $(TARGET)

$(TARGET) : build/output.elf
	$(ARMGNU)-objcopy $< -O binary $@

build/output.elf : $(LINKER) $(OBJECTS)
	$(ARMGNU)-ld $(LDFLAGS) $(OBJECTS) -o $@ -T $(LINKER)

build/%.o : %.s | build
	$(ARMGNU)-as -I . $< -o $@

build/%.o : %.c | build
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

build :
	@mkdir -p $@

clean :
	rm -f $(DEPS) $(OBJECTS) build/output.elf $(TARGET)

run : $(TARGET) qemu/qemu-system-arm qemu/libpng12.so.0 qemu/libcurl.so.4
	@LD_LIBRARY_PATH=qemu qemu/qemu-system-arm -machine raspi2 -bios $(TARGET) -serial mon:stdio -s -S

-include $(DEPS)
