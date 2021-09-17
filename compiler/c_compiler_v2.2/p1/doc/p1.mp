.NM p1 parse C programs
.SY
p1 -[a b# c e l m n# o* r# u] <file>
.FU
p1 is the parsing pass of the C compiler.
It accepts a sequential file of lexemes from the preprocessor pp
and writes a sequential file of flow graphs and parse trees,
suitable for input to a machine-dependent code generator p2.
The operation of p1 is largely independent of any target machine.
The flag options are:
.IP -a
compile code for machines with separate address and data registers.
This flag implies -r6
(because currently used only in conjunction with the MC68000
code generator).
.IP "-b# "
enforce storage boundaries according to #, which is reduced
modulo 4.
A bound of 0 leaves no holes in structures or auto allocations;
a bound of 1 (default) requires short, int and longer data to begin on an even
bound;
a bound of 2 is the same as 1, except that
4-8 byte data are forced to a multiple of four byte boundary;
a bound of 3 is the same as 2, except that 8 byte data (doubles) are forced
to a multiple of eight byte boundary.
.IP -c
ignore case distinctions in testing external identifiers for equality,
and map all names to lowercase on output.
By default, case distinctions matter.
.IP -e
don't force loading of extern references that are declared
but never defined or used in an expression.
Default is to load all externs declared.
.IP "-l"
take integers and pointers to be 4 bytes long.
Default is 2 bytes.
.IP -m
treat each struct/union as a separate name space, and require x.m
to have x a structure with m one of its members.
.IP "-n# "
ignore characters after the first # in testing external identifiers
for equality.
Default is 7;
maximum is 8,
except that values up to 32 are permitted for the VAX-11 code generator only.
.IP "-o*"
write the output to the
file * and write error messages to STDOUT.
Default is STDOUT for output and STDERR for error messages.
.IP "-r# "
assign no more than # variables to registers
at any one time, where # is reduced modulo 7.
Default is 3 register variables;
values above 3 are currently acceptable only for the MC68000
code generator (3 data registers + 3 address registers maximum),
and the VAX-11 code generator (6 registers maximum).
.IP -u
take "string" as array of unsigned char, not array of char.
.LP
If <file> is present, it is used as the input file instead of the default
STDIN.
On many systems (other than Idris/UNIX), the -o option and <file> are mandatory because
STDIN and STDOUT are interpreted as text files, and hence become corrupted.
.EG
p1 is usually sandwiched between pp and some version of p2, as in:

	pp -x -o temp1 file.c
	p1 -o temp2 temp1
	p2.11 -o file.s temp2
.SA
pp
.BU
p1 can be rather cavalier about semicolons.
