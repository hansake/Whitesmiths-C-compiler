.NM pp preprocess defines and includes
.SY
pp -[c d*^ i* o* p? s? x 6] <files>
.FU
.HC
pp is the preprocessor used by the C compiler, to perform #define, #include,
and other functions signalled by a #, before actual compilation begins.
It can be used to advantage, however, with most language processors.
The flag options are:
.IP -c
don't strip out /* comments */ nor continue lines that end with \.
.IP "-d* "
where * has the form name=def,
define name with the definition string def before reading the input;
if =def is omitted, the definition is taken as "1".
The name and def must be in the same argument,
i.e., no blanks are permitted unless the argument is quoted.
Up to ten definitions may be entered in this fashion.
.IP "-i* "
change the prefix used with #include <filename> from the default ""
to the string *.
Multiple prefixes 
to be tried in order
may be specified, separated by the character '|'.
.IP "-o* "
write the output to the file * and write error messages to STDOUT.
Default is STDOUT for output and STDERR for error messages.
On many systems (other than Idris/UNIX), the -o option is mandatory with -x because
STDOUT is interpreted as a text file, and hence becomes corrupted.
.IP -p?
change the preprocessor control character from '#' to the character ?.
.IP -s?
change the secondary preprocessor control character from '@' to the character ?.
.IP -x
put out lexemes for input to the C compiler p1, not lines of text.
.IP -6
put out extra newlines and/or SOH ('\1') codes to keep source line numbers correct
for UNIX/V6 compiler or ptc.
.LP
pp processes the named files, or STDIN if none are given, in the order specified,
writing the resultant text to STDOUT.
Preprocessor actions are described in detail in 
Section I of the C Programmers' Manual.
.LP
The presence of a secondary preprocessor control character permits
two levels of parameterization.
For instance, the invocation
.DS
	pp -c -p@
.DE
.LP
will expand define and ifdef conditionals, leaving all # commands
and comments intact;
invoking pp with no arguments would expand both @ and # commands.
The flag -s# would effectively disable the secondary control character.
.EG
The standard style for writing C programs is:

	/* name of program
	 */
	#include <std.h>

	#define MAXN	100

	COUNT things[MAXN];
	etc.

.FI
The use of uppercase only identifiers is not required by pp,
but is strongly recommended to distinguish parameters from normal
program identifiers and keywords.
.SA
p1, ptc
.BU
Unbalanced quotes ' or " may not occur in a line, even in the absence
of the -x flag.  Floating constants longer than 38 digits may compile
incorrectly,
on some host machines.
.HC @
