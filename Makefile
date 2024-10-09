CC = gcc
ASM = nasm
LD = ld
CFLAGS = -g -c -I./include -ffreestanding -fno-stack-protector -m32
LDFLAGS = -T linker.ld -m elf_i386
ASMFLAGS = -f elf32
QEMU = qemu-system-i386

ISO = grim_os.iso
ISO_DIR = iso
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub
BUILD_DIR = build
QEMUFLAGS = -cdrom $(ISO) -m 2G -serial stdio

C_SOURCES := $(shell find src -name '*.c')
ASM_SOURCES := $(shell find src -name '*.asm')

C_OBJECTS := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst src/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

all: iso $(ISO)

iso: kernel.elf
	@mkdir -p $(GRUB_DIR)
	cp $(BUILD_DIR)/kernel.elf $(BOOT_DIR)/
	@echo "set timeout=0" > $(GRUB_DIR)/grub.cfg
	@echo "set default=0" >> $(GRUB_DIR)/grub.cfg
	@echo "" >> $(GRUB_DIR)/grub.cfg
	@echo "menuentry \"grim_os\" {" >> $(GRUB_DIR)/grub.cfg
	@echo "    multiboot /boot/kernel.elf" >> $(GRUB_DIR)/grub.cfg
	@echo "    boot" >> $(GRUB_DIR)/grub.cfg
	@echo "}" >> $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Compile assembly files and output to build directory
$(BUILD_DIR)/%.o: src/%.asm
	@mkdir -p $(dir $@)  # Ensure directory exists for object file
	$(ASM) -f elf32 $< -o $@

# Compile C files and output to build directory
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)  # Ensure directory exists for object file
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything together into a kernel binary in the build directory
kernel.elf: $(ASM_OBJECTS) $(C_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.elf $(ASM_OBJECTS) $(C_OBJECTS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) *.iso

# Clean only object files
clean-objects:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/kernel.elf

# Run the kernel in QEMU
run: all
	$(QEMU) $(QEMUFLAGS)

.PHONY: all iso clean run clean-objects