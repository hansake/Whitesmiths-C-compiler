#!/bin/sh
# Write multiple binary files to CP/M disk image for Z80SIM
#
dsk=$1
shift
for fname in "$@"
do
    cpmw.sh $dsk "$fname"
done
