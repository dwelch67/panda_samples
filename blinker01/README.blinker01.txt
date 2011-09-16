See the top level README file for information about the users guide
and schematic.s

The schematics show that p0.16 drives an led.  This is the
led we are going to blink with this program.

This first blinker example is ARM assembler only.  You will need
binutils (assembler and linker) but not gcc (C compiler) to build this.

Looking at the users guide for this part there are legacy or slow
registers for GPIO, and some new fast registers.  The first example
will use the legacy registers.

We need to set the direction bit to a 1 for port 16 and then use
set and clear to set and clear the output.  Slowing the led blink
rate down so we humans can see it, is done using a simple counter.

Loading the program and running it is described in the top level
README.


