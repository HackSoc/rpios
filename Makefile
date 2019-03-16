ARMGNU ?= arm-none-eabi
ARMGNUVERSION ?= 6.3.1
LINKER ?= linker/kernel.ld

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

AOBJS := $(patsubst src/%.s,build/%.o,$(call rwildcard, src/, *.s))
COBJS := $(patsubst src/%.c,build/%.o,$(call rwildcard, src/, *.c))
OBJECTS := $(AOBJS) $(COBJS)
DEPS := $(COBJS:.o=.d)

CFLAGS += -std=c99 -mfloat-abi=hard -ffreestanding -fno-builtin -march=armv7-a -MD -MP -g

LDFLAGS += --no-undefined -L/usr/lib/gcc/arm-none-eabi/$(ARMGNUVERSION)/

TARGET = build/kernel.img

.PHONY: clean run all debug

all : $(TARGET)

$(TARGET) : build/output.elf
	$(ARMGNU)-objcopy $< -O binary $@

build/output.elf : $(LINKER) $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) $(LDFLAGS) -o $@ -T $(LINKER)

build/%.o : src/%.s | build
	$(ARMGNU)-as -I . $< -o $@

build/%.o : src/%.c | build
	@mkdir -p $(dir $@)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

build :
	@mkdir -p $@

clean :
	rm -f $(DEPS) $(OBJECTS) build/output.elf $(TARGET)

run : $(TARGET) qemu/qemu-system-arm qemu/libpng12.so.0 qemu/libcurl.so.4
	@LD_LIBRARY_PATH=qemu qemu/qemu-system-arm -machine raspi2 -bios $(TARGET) -serial mon:stdio -s -S

debug : $(TARGET)
	gdb-multiarch build/output.elf --ex "target remote localhost:1234" --ex "set debug-file-directory build"

-include $(DEPS)-L/usr/lib/gcc/arm-none-eabi/6.3.1/
