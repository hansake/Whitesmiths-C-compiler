.NM as.80 A-Natural assembler for 8080
.SY
as.80 -[o* x] <files>
.FU
as.80 assembles A-Natural narrative assembly language for the Intel 8080
to standard format relocatable object images.
Since the output of the 8080 code generator p2.80 is A-Natural, as.80 is
required to produce relocatable images suitable for
binding with link.
.LP
The flags are:
.IP "-o* "
write the output to the file *.
Default is xeq.
Under some circumstances,
an input filename can take the place of this option,
as explained below.
.IP "-x "
place in the object image only those symbols that are undefined or that
are to be made globally known.
Currently, this happens anyway; the flag is present for compatibility
with other assemblers.
.LP
If <files> are present,
they are concatenated in order and used as the input
file instead of the default STDIN.
.LP
If -o is absent,
and one or more files are present,
and the first filename begins with 's',
then as.80 behaves as if -o were specified using the first
filename,
only with the trailing 's' changed to 'o'.
Thus,
.DS
	as.80 file.s

is the same as

	as.80 -o file.o file.s
.DE
.LP
A relocatable object image consists of an eight-word header
followed by
a text segment, a data segment, the symbol table, and relocation information.
The header consists of the value 0x1499, the number of symbol table bytes,
the number of bytes of object code defined by the text segment,
the number of bytes defined by the data segment,
three zero words,
and the data segment offset, which always equals the text segment size.
All words in the object image are written less significant byte first.
The text segment is relocated relative to location zero, the data
segment is relocated relative to the end of the text segment.
.LP
Relocation information consists of two byte streams, one for the text
segment and one for the data segment, each terminated by a zero control byte.
Control bytes in the range [1, 31] cause that many bytes in the corresponding
segment to be skipped;
bytes in the range [32, 63] skip 32 bytes, plus 256 times the control byte
minus 32, plus the number of bytes specified by the relocation byte following.
.LP
All other control bytes control relocation of the next word in the
corresponding segment.
The 1-weighted bit and 2-weighted bit are both always zero for the 8080;
the symbol code is the control byte right shifted two places, minus 16.
.LP
A symbol code of 47 is replaced by a code obtained from the byte or bytes
following in the relocation stream.
If the next byte is less than 128, then the symbol code is its value plus 47;
otherwise the code is that byte minus 128, times 256, plus 175 plus the
value of the next relocation byte after that one.
.LP
A symbol code of zero calls for no further relocation;
1 means that a change in text bias must be added to the word;
2 means that a change in data bias must be added;
3 is not used.
Other symbol codes call for the value of the symbol table entry indexed
by the symbol code minus 4 to be added to the word.
.LP
Each symbol table entry consists of a value word, a flag byte,
and a nine-byte name padded with trailing NULs.
Meaningful flag values are 0 for undefined, 4 for defined absolute,
5 for defined text relative, and 6 for defined data relative.
To this is added 010 if the symbol is to be globally known.
.SA
link, p2.80, rel
