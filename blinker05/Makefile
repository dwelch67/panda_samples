
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts



all : blinker05.gcc.bin blinker05.clang.norm.bin blinker05.clang.opt.bin


startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o



blinker05.gcc.bin : memmap startup.o blinker05.gcc.o
	$(ARMGNU)-ld -T memmap startup.o blinker05.gcc.o -o blinker05.gcc.elf
	$(ARMGNU)-objdump -D blinker05.gcc.elf > blinker05.gcc.list
	$(ARMGNU)-objcopy blinker05.gcc.elf blinker05.gcc.bin -O binary

blinker05.gcc.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -c blinker05.c -o blinker05.gcc.o



blinker05.clang.bc : blinker05.c
	clang $(LOPS) -c blinker05.c -o blinker05.clang.bc

blinker05.clang.norm.bin : memmap startup.o blinker05.clang.bc
	llc $(LLCOPS) blinker05.clang.bc -o blinker05.clang.norm.s
	$(ARMGNU)-as blinker05.clang.norm.s -o blinker05.clang.norm.o
	$(ARMGNU)-ld -o blinker05.clang.norm.elf -T memmap startup.o blinker05.clang.norm.o
	$(ARMGNU)-objdump -D blinker05.clang.norm.elf > blinker05.clang.norm.list
	$(ARMGNU)-objcopy blinker05.clang.norm.elf blinker05.clang.norm.bin -O binary

blinker05.clang.opt.bin : memmap startup.o blinker05.clang.bc
	opt $(OOPS) blinker05.clang.bc -o blinker05.clang.opt.bc
	llc $(LLCOPS) blinker05.clang.opt.bc -o blinker05.clang.opt.s
	$(ARMGNU)-as blinker05.clang.opt.s -o blinker05.clang.opt.o
	$(ARMGNU)-ld -o blinker05.clang.opt.elf -T memmap startup.o blinker05.clang.opt.o
	$(ARMGNU)-objdump -D blinker05.clang.opt.elf > blinker05.clang.opt.list
	$(ARMGNU)-objcopy blinker05.clang.opt.elf blinker05.clang.opt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s



