#!/bin/sh
#
# Boot MS-DOS 6.22 in a QEMU virtual machine
# the VM contains Whitesmiths C compiler for IBM System/36
#
qemu-system-i386 -m 16 \
-drive file=c-s36-msdos.img,format=raw,index=0,media=disk \
-rtc base=localtime -boot order=c &
