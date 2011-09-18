
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts



all : blinker04.gcc.bin blinker04.clang.norm.bin blinker04.clang.opt.bin


startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o



blinker04.gcc.bin : memmap startup.o blinker04.gcc.o
	$(ARMGNU)-ld -T memmap startup.o blinker04.gcc.o -o blinker04.gcc.elf
	$(ARMGNU)-objdump -D blinker04.gcc.elf > blinker04.gcc.list
	$(ARMGNU)-objcopy blinker04.gcc.elf blinker04.gcc.bin -O binary

blinker04.gcc.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -c blinker04.c -o blinker04.gcc.o



blinker04.clang.bc : blinker04.c
	clang $(LOPS) -c blinker04.c -o blinker04.clang.bc

blinker04.clang.norm.bin : memmap startup.o blinker04.clang.bc
	llc $(LLCOPS) blinker04.clang.bc -o blinker04.clang.norm.s
	$(ARMGNU)-as blinker04.clang.norm.s -o blinker04.clang.norm.o
	$(ARMGNU)-ld -o blinker04.clang.norm.elf -T memmap startup.o blinker04.clang.norm.o
	$(ARMGNU)-objdump -D blinker04.clang.norm.elf > blinker04.clang.norm.list
	$(ARMGNU)-objcopy blinker04.clang.norm.elf blinker04.clang.norm.bin -O binary

blinker04.clang.opt.bin : memmap startup.o blinker04.clang.bc
	opt $(OOPS) blinker04.clang.bc -o blinker04.clang.opt.bc
	llc $(LLCOPS) blinker04.clang.opt.bc -o blinker04.clang.opt.s
	$(ARMGNU)-as blinker04.clang.opt.s -o blinker04.clang.opt.o
	$(ARMGNU)-ld -o blinker04.clang.opt.elf -T memmap startup.o blinker04.clang.opt.o
	$(ARMGNU)-objdump -D blinker04.clang.opt.elf > blinker04.clang.opt.list
	$(ARMGNU)-objcopy blinker04.clang.opt.elf blinker04.clang.opt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s



