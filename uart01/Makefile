
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts



all : uart01.gcc.bin uart01.clang.norm.bin uart01.clang.opt.bin


startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o



uart01.gcc.bin : memmap startup.o uart01.gcc.o
	$(ARMGNU)-ld -T memmap startup.o uart01.gcc.o -o uart01.gcc.elf
	$(ARMGNU)-objdump -D uart01.gcc.elf > uart01.gcc.list
	$(ARMGNU)-objcopy uart01.gcc.elf uart01.gcc.bin -O binary

uart01.gcc.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -c uart01.c -o uart01.gcc.o



uart01.clang.bc : uart01.c
	clang $(LOPS) -c uart01.c -o uart01.clang.bc

uart01.clang.norm.bin : memmap startup.o uart01.clang.bc
	llc $(LLCOPS) uart01.clang.bc -o uart01.clang.norm.s
	$(ARMGNU)-as uart01.clang.norm.s -o uart01.clang.norm.o
	$(ARMGNU)-ld -o uart01.clang.norm.elf -T memmap startup.o uart01.clang.norm.o
	$(ARMGNU)-objdump -D uart01.clang.norm.elf > uart01.clang.norm.list
	$(ARMGNU)-objcopy uart01.clang.norm.elf uart01.clang.norm.bin -O binary

uart01.clang.opt.bin : memmap startup.o uart01.clang.bc
	opt $(OOPS) uart01.clang.bc -o uart01.clang.opt.bc
	llc $(LLCOPS) uart01.clang.opt.bc -o uart01.clang.opt.s
	$(ARMGNU)-as uart01.clang.opt.s -o uart01.clang.opt.o
	$(ARMGNU)-ld -o uart01.clang.opt.elf -T memmap startup.o uart01.clang.opt.o
	$(ARMGNU)-objdump -D uart01.clang.opt.elf > uart01.clang.opt.list
	$(ARMGNU)-objcopy uart01.clang.opt.elf uart01.clang.opt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s



