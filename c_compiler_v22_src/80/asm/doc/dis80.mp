.hc
.NM dis80 translate A-Natural relocatable files
.SY
dis80 -[c i] <files>
.FU
dis80 permits translation to symbolic form of
the relocatable binary files produced by the A-Natural assembler as.80.
This form is compatible with existing assembler code on an 8080.
.LP
The flags are:
.IP "-c "
Translate each file to an assembler program suitable for input
to the Microsoft assembler under CP/M.
Each underscore '_' in an identifier is changed to '.',
and each '.' is changed to '$'.
.IP "-i "
Translate each file to an assembler program suitable for input
to the asm80 assembler under ISIS-II.
Each underscore '_' in an identifier is changed to '?',
and each '.' is changed to '@'.
.LP
If no flags are given, the default is -c.
.LP
<files> specifies one or more files, which must be in A-Natural relocatable
format.
If more than one file is specified, then the output for each is preceded
by a line of the form
.DS
	file:
.DE
.LP
Assembler output is headed by a title directive
(name for ISIS-II), where the name is derived from the filename by stripping
off any characters to the right of and including the leftmost '.',
then appending a '@' in the case of ISIS-II,
and a '$' in the case of Microsoft.
.RT
dis80 returns success if no diagnostics
are produced, i.e. if all reads are successful and all file formats are valid.
Otherwise it returns failure.
.EG
To convert chdr.o for use with Microsoft assembler code:
.DS
	% dis80 chdr.o >chdr.mac
.DE
.SA
as.80, link
.BU
The symbols "_c_" and "_d_" must not be used in
A-Natural programs to be translated with dis80.
