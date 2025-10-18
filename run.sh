
nasm -f elf32 multiboot_header.asm -o multiboot_header.o
g++ -ffreestanding -m32 -c kernel.cpp -o kernel.o
g++ -ffreestanding -m32 -c console.cpp -o console.o
g++ -ffreestanding -m32 -c keyboard.cpp -o keyboard.o
g++ -ffreestanding -m32 -c hardware.cpp -o hardware.o
g++ -ffreestanding -m32 -c clock.cpp -o clock.o
ld -m elf_i386 -T linker.ld -o kernel.elf console.o multiboot_header.o kernel.o keyboard.o hardware.o clock.o
mkdir -p isodir/boot/grub
cp kernel.elf isodir/boot/
cat > isodir/boot/grub/grub.cfg << 'EOF'
set timeout=1
menuentry "BuizlOS" {
    multiboot /boot/kernel.elf
    boot
}
EOF
grub2-mkrescue -o buizlos.iso isodir
qemu-system-i386 -cdrom buizlos.iso
