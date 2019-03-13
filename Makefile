ARMGNU ?= arm-none-eabi
LINKER ?= build/kernel.ld

AOBJS := $(patsubst %.s,build/%.o,$(wildcard *.s))
COBJS := $(patsubst %.c,build/%.o,$(wildcard *.c) $(wildcard demos/*.c))
OBJECTS := $(AOBJS) $(COBJS)
DEPS := $(COBJS:.o=.d)

CFLAGS += -std=gnu99 -mfloat-abi=hard -ffreestanding -fno-builtin -march=armv7-a -mfpu=vfpv3 -I. -MD -MP -g
LDFLAGS += --no-undefined -L/usr/lib/gcc/arm-none-eabi/8.3.0/ -lgcc -g

TARGET = build/kernel.img

.PHONY: clean run all

all : $(TARGET)

$(TARGET) : build/output.elf
	$(ARMGNU)-objcopy $< -O binary $@

build/output.elf : $(LINKER) $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) $(LDFLAGS) -o $@ -T $(LINKER)

build/%.o : %.s | build
	$(ARMGNU)-as -I . $< -o $@

build/%.o : %.c | build
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

build :
	@mkdir -p $@
	@mkdir -p $@/demos

clean :
	rm -f $(DEPS) $(OBJECTS) build/output.elf $(TARGET)

run : $(TARGET)
	qemu-system-arm -machine raspi2 -bios $(TARGET) -serial mon:stdio -s -S

-include $(DEPS)
