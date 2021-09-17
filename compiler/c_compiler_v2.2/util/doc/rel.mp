.NM rel examine relocatable files
.SY
rel <flags> <files>
.FU
rel permits inspection of
the relocatable binary files produced by the assembler as
or by the loader ld11.
It can be used simply to check for defined or undefined symbols, or to
check module sizes.
.LP
The flags are:
.IP "-d "
List all defined symbols in each file, along with the value of the symbol,
in octal,
and the code 'T' for text relative, 'D' for data relative, 'B'
for bss relative, 'A' for
absolute, 'F' for filenames, or '?'
for anything else it doesn't recognize.
Lower case letters are used for local symbols,
upper case for globals.
.IP "-i "
Print global entries with intervals shown in the value field.
-i implies
sorting by value plus the -d and -u flags.
.IP "-s "
Display the sizes, in decimal, of the text segment, the data segment,
the bss segment,
and the sum of both segments.
.IP "-u "
List all undefined symbols in each file.
If -d is also specified, the symbol is listed with
the code 'U'; its value is the space required.
.IP "-v "
Sort by value, implies the -d flag above.
If the values match then the names are used.
.LP
If no flags are given, the default is -du,
i.e. a list of all symbols with their values is given, in alphabetical
order by symbol name.
If multiple options are given, the order of output is 1) the segment
sizes called for by -s,
2) the symbol list called for by -d and/or -u.
.LP
<files> specifies zero or more files, which must be in relocatable
format.
If more than one file is specified, then the output for each is preceded
by a line of the form
.DS
	file:
.DE
.LP
and if -s is present
a total line is also printed.
If no files are specified, or if '-' is encountered on the command line,
"a.out" is used.
.RT
rel returns success if no diagnostics
are produced, i.e. if all reads are successful and all file formats are valid.
.EG
To obtain a list of undefined symbols:

% rel -u mesg.o

which might look like:

_chmod
_cpybuf
_devname
_fstat
c~ret
c~sav
.SA
as, ld
