.hc
.NM anat A-Natural translator
.SY
anat -[i o* s*] <files>
.FU
anat translates A-Natural narrative assembly language for the Intel 8080
to standard assembly language as accepted by either the ISIS-II asm80
or the Microsoft Macro-80 assembler.
Since the output of the 8080 code generator p2.80 is A-Natural, anat is
required to interface the C compiler to existing 8080 software development
facilities.
.LP
The flags are:
.IP "-i "
emit code in ISIS-II format, in which
identifiers have each '.' changed to '@' and each '_' changed to '?'.
Default is Microsoft format,
in which each '.' is changed to '$' and each '_' is changed to '.'.
.IP "-o* "
write the output to the file * and write error messages to STDOUT.
Default is STDOUT for output and STDERR for error messages.
.IP "-s* "
emit an end statement with the operand *, to specify a starting address.
Default is to emit an end statement with no start address.
.LP
If <files> are present,
they are concatenated in order and used as the input
file instead of the default STDIN.
.LP
If -o is absent,
and one or more files are present,
and the first filename ends with '8',
then anat behaves as if -o was specified using the first
filename,
only with the trailing '8' changed to 'm'.
Thus,
.DS
	anat file.8
.DE
.LP
is the same as
.DS
	anat -o file.m file.8
.DE
.LP
If -o is present, a name directive (title for Microsoft)
is emitted using the output
file name as the module name;
a .suffix in the name is stripped off.
If there is no -o, but <files> are present,
then the first input file name is used
in the name directive.
If neither is present, no name directive is emitted.
.LP
Code generation begins in cseg, and reverts to cseg at the end before any
literals are emitted.
Any symbols not defined by the end of the input file are published via
extrn statements at the end.
.BU
The start string used with -s does not have '.' and '_' mapped
as for identifiers.
