include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
GCCFLAGS=-g -m64 -fno-exceptions -fno-asynchronous-unwind-tables -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -mno-red-zone -Wall -ffreestanding -nostdlib -fno-common -std=c99


LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)

all: $(KERNEL)

$(KERNEL): $(STATICLIBS) $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel.elf --oformat=elf64-x86-64 $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf $(ALL_OBJECTS) *.bin

.PHONY: all clean