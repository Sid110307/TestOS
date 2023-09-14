CC =			clang
CXX =			clang++
ARCH =			$(shell $(CC) -dumpmachine | cut -f1 -d-)

SRC_DIR =		src
DATA_DIR =		data
BUILD_DIR =		bin

OUTPUT =		$(BUILD_DIR)/kernel.elf
BOOTLOADER =	gnu-efi/$(ARCH)/bootloader/main.efi

ASMFLAGS =		-f elf64
CFLAGS =		-m64 -c -O2 -Wall -Wextra -ffreestanding -fshort-wchar -mno-red-zone
LDFLAGS =		-m elf_$(ARCH) -T $(DATA_DIR)/linker.ld -Bsymbolic -nostdlib
QEMUFLAGS =		-m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(DATA_DIR)/OVMF_CODE.fd",readonly=on \
				-drive if=pflash,format=raw,unit=1,file="$(DATA_DIR)/OVMF_VARS.fd",readonly=on -net none

ASM_SOURCES =	$(wildcard $(SRC_DIR)/asm/*.asm)
C_SOURCES =		$(wildcard $(SRC_DIR)/*.c)
CPP_SOURCES =	$(wildcard $(SRC_DIR)/*.cpp)
FONTS =			$(wildcard $(SRC_DIR)/lib/*.psf)

ASM_OBJECTS =	$(patsubst $(SRC_DIR)/asm/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
C_OBJECTS =		$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS =	$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

.PHONY: all bootloader build iso clean super-clean
all: bootloader $(OUTPUT) build iso

bootloader:
	mkdir -p $(BUILD_DIR)
	cd gnu-efi && $(MAKE) all bootloader && cd ..

build: $(OUTPUT)
	dd if=/dev/zero of=$(BUILD_DIR)/os.img bs=512 count=93750
	mformat -i $(BUILD_DIR)/os.img -f 1440 ::
	mmd -i $(BUILD_DIR)/os.img ::/EFI
	mmd -i $(BUILD_DIR)/os.img ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/os.img $(BOOTLOADER) ::/EFI/BOOT/BOOT.EFI
	mcopy -i $(BUILD_DIR)/os.img $(OUTPUT) ::
	mcopy -i $(BUILD_DIR)/os.img $(DATA_DIR)/startup.nsh ::
	mcopy -i $(BUILD_DIR)/os.img $(FONTS) ::
	qemu-system-$(ARCH) $(QEMUFLAGS) -drive format=raw,file=$(BUILD_DIR)/os.img

iso: $(OUTPUT)
	mkdir -p $(BUILD_DIR)/iso/EFI/BOOT
	cp $(BOOTLOADER) $(BUILD_DIR)/iso/EFI/BOOT/BOOTX64.EFI
	cp $(OUTPUT) $(BUILD_DIR)/iso/kernel.elf
	cp $(DATA_DIR)/startup.nsh $(BUILD_DIR)/iso/startup.nsh
	cp $(FONTS) $(BUILD_DIR)/iso/
	grub-mkrescue -o $(BUILD_DIR)/os.iso $(BUILD_DIR)/iso

$(BUILD_DIR)/%.o: $(SRC_DIR)/asm/%.asm
	nasm $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.elf: $(ASM_OBJECTS) $(C_OBJECTS) $(CPP_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(BOOTLOADER)

super-clean:
	rm -rf $(BUILD_DIR)
	cd gnu-efi && $(MAKE) clean && cd ..
