
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts



all : blinker03.gcc.bin blinker03.clang.norm.bin blinker03.clang.opt.bin


startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o



blinker03.gcc.bin : memmap startup.o blinker03.gcc.o
	$(ARMGNU)-ld -T memmap startup.o blinker03.gcc.o -o blinker03.gcc.elf
	$(ARMGNU)-objdump -D blinker03.gcc.elf > blinker03.gcc.list
	$(ARMGNU)-objcopy blinker03.gcc.elf blinker03.gcc.bin -O binary

blinker03.gcc.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.gcc.o



blinker03.clang.bc : blinker03.c
	clang $(LOPS) -c blinker03.c -o blinker03.clang.bc

blinker03.clang.norm.bin : memmap startup.o blinker03.clang.bc
	llc $(LLCOPS) blinker03.clang.bc -o blinker03.clang.norm.s
	$(ARMGNU)-as blinker03.clang.norm.s -o blinker03.clang.norm.o
	$(ARMGNU)-ld -o blinker03.clang.norm.elf -T memmap startup.o blinker03.clang.norm.o
	$(ARMGNU)-objdump -D blinker03.clang.norm.elf > blinker03.clang.norm.list
	$(ARMGNU)-objcopy blinker03.clang.norm.elf blinker03.clang.norm.bin -O binary

blinker03.clang.opt.bin : memmap startup.o blinker03.clang.bc
	opt $(OOPS) blinker03.clang.bc -o blinker03.clang.opt.bc
	llc $(LLCOPS) blinker03.clang.opt.bc -o blinker03.clang.opt.s
	$(ARMGNU)-as blinker03.clang.opt.s -o blinker03.clang.opt.o
	$(ARMGNU)-ld -o blinker03.clang.opt.elf -T memmap startup.o blinker03.clang.opt.o
	$(ARMGNU)-objdump -D blinker03.clang.opt.elf > blinker03.clang.opt.list
	$(ARMGNU)-objcopy blinker03.clang.opt.elf blinker03.clang.opt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s



