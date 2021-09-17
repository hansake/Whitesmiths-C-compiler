#!/bin/sh
# Write multiple text files to CP/M disk image for Z80SIM
#
dsk=$1
shift
for fname in "$@"
do
    cpmw.sh -t $dsk "$fname"
done
