#!/bin/sh
DoExitAsm ()
{ echo "An error occurred while assembling $1"; exit 1; }
DoExitLink ()
{ echo "An error occurred while linking $1"; exit 1; }
echo Linking /home/student/Документы/330/Программирование/Долги/2laba/absractfinder
OFS=$IFS
IFS="
"
/usr/bin/ld.bfd -b elf64-x86-64 -m elf_x86_64       -L. -o '/home/student/Документы/330/Программирование/Долги/2laba/absractfinder' -T '/home/student/Документы/330/Программирование/Долги/2laba/link5968.res' -e _start
if [ $? != 0 ]; then DoExitLink /home/student/Документы/330/Программирование/Долги/2laba/absractfinder; fi
IFS=$OFS
