all: fdoomfw.elf fdoomfw.bin fdoomfw.h

fdoomfw.elf: fdoomfw.c fdoomfw.lds fharddoom.h
	riscv64-linux-gnu-gcc -O2 -ffreestanding -nostdlib fdoomfw.c -o fdoomfw.elf -march=rv32im -mabi=ilp32 -Wl,-T,fdoomfw.lds,--build-id=none

fdoomfw.bin: fdoomfw.elf
	riscv64-linux-gnu-objcopy fdoomfw.elf -O binary fdoomfw.bin

fdoomfw.h: fdoomfw.bin
	python mkhdr.py fdoomfw.bin fdoomfw.h
