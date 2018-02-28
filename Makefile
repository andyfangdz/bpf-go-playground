uname=$(shell uname -r)

build-elf:
	$(CC) \
		-D__KERNEL__ \
		-D__ASM_SYSREG_H \
		-O2 -emit-llvm -c open-trace.c \
		-I /usr/include/x86_64-linux-gnu \
		-I /usr/src/linux-headers-$(uname)/include \
		-I /usr/src/linux-headers-$(uname)/arch/x86/include \
		-I /usr/src/linux-headers-$(uname)/arch/x86/include/generated \
		-emit-llvm -o - | \
		llc -march=bpf -filetype=obj -o program.o
