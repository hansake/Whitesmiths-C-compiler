/*	CONVERT IDRIS BINARY TO HEX OR S-RECORDS
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define DFLTFIL	"xeq"

/*	flags:
	+#		start with byte #
	-#		only output every # bytes
	-db##	override object module data bias with ##
	-dr		produce Digital Research 8086 records
	-h		don't put header for hex
	-m*		replace default module name with *
	-r##	raw data file, don't interpert as Whitesmiths file
	-s		produce S records, not hex
	-tb##	override object module text bias with ##
 */
BOOL drfl {NO};
BOOL hfl {NO};
BOOL sfl {NO};
BYTES skew {1};
BYTES soff {0};
LONG rfl {-1};
TEXT *mfl {NULL};
ULONG db {-1};
ULONG tb {-1};

TEXT *_pname {"hex"};

/*	other "globals"
 */
BOOL longint {NO};
BOOL lsfmt {NO};
FILE ifd {0};
FIO ifio {0};
TEXT *iname {NULL};
ULONG dbias {0};
ULONG dsize {0};
ULONG loc {0};
ULONG tbias {0};
ULONG tsize {0};

/*	fill up to tot bytes in buf using skew 
 */
COUNT filbuf(fio, buf, tot, sk, amt)
	FIO *fio;
	TEXT *buf;
	COUNT tot;
	BYTES sk;
	LONG *amt;
	{
	FAST COUNT i, j, stat;
	UTINY trash;

	for (i = j = 0; *amt && j < tot; *amt =- 1)
		{
		if (i % skew == 0)
			{
			*buf++ = gtc(fio);
			++j;
			}
		else
			trash = gtc(fio);
		++i;
		}
	while (*amt && i % skew)
		{
		trash = gtc(fio);
		++i;
		*amt =- 1;
		}
	return (j);
	}

/*	get needed things from header in a portable way
*/
VOID gethdr()
	{
	IMPORT BOOL longint;
	IMPORT FIO ifio;
	IMPORT LONG xstol();
	IMPORT UCOUNT xstos();
	IMPORT ULONG dbias, dsize, tbias, tsize;
	FAST COUNT i;
	FAST TEXT *p;
	COUNT hsize = (longint) ? 26 : 14;
	TEXT buf[26];

	for (i = hsize, p = buf; i ; --i)
		*p++ = gtc(&ifio);
	if (longint)
		{
		tsize = xstol(&buf[2]);
		dsize = xstol(&buf[6]);
		tbias = xstol(&buf[18]);
		dbias = xstol(&buf[22]);
		}
	else
		{
		tsize = xstos(&buf[2]);
		dsize = xstos(&buf[4]);
		tbias = xstos(&buf[10]);
		dbias = xstos(&buf[12]);
		}
	}

/*	get magic # at start of file
 */
VOID getmagic()
	{
	IMPORT BOOL longint, lsfmt;
	IMPORT FIO ifio;
	UTINY header[2];

	header[0] = gtc(&ifio);
	header[1] = gtc(&ifio);
	if (header[0] != 0x99)
		error("bad file format", NULL);
	longint = header[1] & 010;
	lsfmt = header[1] & 020;
	}

/*	translate abs records to records
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL drfl, hfl, sfl;
	IMPORT BYTES skew, soff;
	IMPORT FILE ifd;
	IMPORT FIO ifio, stdout;
	IMPORT LONG rfl;
	IMPORT TEXT *iname, *mfl;
	IMPORT ULONG db, dbias, dsize, loc, tb, tbias, tsize;
	FAST BYTES i, n;
	TEXT buf[32];

	getflags(&ac, &av, "db##,dr,h,m*,r##,s,tb##,+#,#:F <ifile>", &db, &drfl,
		&hfl, &mfl, &rfl, &sfl, &tb, &soff, &skew);
	ifd = getbfiles(&ac, &av, STDIN, STDERR, YES);
	if (ifd != STDIN)
		iname = av[-1];
	else if ((ifd = open(iname = DFLTFIL, READ, 1)) < 0)
		ifd = STDERR;
	if (ifd == STDERR)
		error("can't read ", iname);
	if (skew <= soff)
		error("bad starting offset", NULL);
	if (mfl)
		iname = mfl;
	finit(&stdout, STDOUT, BWRITE);
	if (rfl == -1)
		{
		finit(&ifio, ifd, READ);
		getmagic();
		gethdr();
		for (i = 0; tsize && i < soff; ++i, --tsize)
			buf[0] = gtc(&ifio);
		if (skew == 1)
			loc = ((tb != -1) ? tb : tbias) + soff;
		else
			loc = (tb != -1) ? tb : 0;
		tbias = loc;
		putstart();
		while (0 < tsize)
			{
			putrec(loc, buf, i = filbuf(&ifio, buf, 32, skew, &tsize), YES);
			loc =+ i;
			}
		for (i = 0; dsize && i < soff; ++i, --dsize)
			buf[0] = gtc(&ifio);
		if (skew == 1)
			loc = ((db != -1) ? db : dbias) + soff;
		else
			loc = (db != -1) ? db : 0;
		while (0 < dsize)
			{
			putrec(loc, buf, i = filbuf(&ifio, buf, 32, skew, &dsize), NO);
			loc =+ i;
			}
		}
	else
		{
		loc = tbias = rfl;
		putstart();
		while (n = fread(ifd, buf, 32))
			{
			putrec(loc, buf, n, YES);
			loc =+ n;
			}
		}
	putend();
	}

/*	build a hex record and put it out
 */
BYTES putrec(off, s, n, iscode)
	ULONG off;
	FAST UTINY *s;
	COUNT n;
	BOOL iscode;
	{
	IMPORT BOOL drfl, sfl;
	FAST COUNT i, sum;
	BOOL islong = (sfl && (off >> 16));
	COUNT m;
	TEXT buf[80];

	m = n;
	if (sfl)
		{
		cpystr(buf, islong ? "S2" : "S1", "XX", NULL);
		sum = putx(&buf[2], n + ((islong) ? 4 : 3));
		i = 4;
		if (islong)
			{
			sum =+ putx(buf + i, (UCOUNT)(off >> 16));
			i =+ 2;
			}
		}
	else
		{
		cpystr(buf, ":XXXXXX", NULL);
		sum = putx(&buf[1], n);
		i = 3;
		}
	sum =+ putx(buf + i, (UCOUNT)(off >> 8));
	i =+ 2;
	sum =+ putx(buf + i, (UCOUNT)off);
	i =+ 2;
	if (!sfl)
		{
		sum =+ putx(buf + i, !drfl ? 0 : iscode ? 0x81 : 0x82);
		i =+ 2;
		}
	for (; 0 < n; --n, ++s, i =+ 2)
		sum =+ putx(buf + i, *s);
	putx(buf + i, (sfl) ? ~sum : -sum);
	buf[i + 2] = '\n';
	if (putlin(buf, i + 3) != i + 3)
		error("can't write data records", NULL);
	return (m);
	}


/*	put start record
 */
VOID putstart()
	{
	IMPORT BOOL hfl, sfl;
	IMPORT TEXT *iname;
	FAST COUNT i, sum;
	FAST TEXT *s;
	TEXT buf[80];

	if (!sfl)
		if (!hfl)
			{
			cpystr(buf, "$\n", NULL);
			i = 2;
			}
		else 
			i = 0;
	else
		{
		cpystr(buf, "S0", "XX0000", NULL);
		sum = putx(&buf[2], lenstr(iname) + 3);
		for (i = 8, s = iname; *s; i =+ 2, ++s)
			sum =+ putx(buf + i, *s);
		putx(buf + i, ~sum);
		buf[i + 2] = '\n';
		i =+ 3;
		}
	if (putlin(buf, i) != i)
		error("can't write start record", NULL);
	}

/*	put end record with start address
 */
VOID putend()
	{
	IMPORT BOOL sfl;
	IMPORT ULONG tbias;
	BOOL islong = (sfl && (tbias >> 16));
	FAST COUNT i, sum;
	TEXT buf[16];

	if (!sfl)
		{
		cpystr(buf, ":00XXXX01XX", NULL);
		sum = 1;
		sum =+ putx(&buf[3], (COUNT)(tbias >> 8));
		sum =+ putx(&buf[5], (COUNT)tbias);
		putx(&buf[9], -sum);
		buf[11] = '\n';
		i = 12;
		}
	else
		{
		cpystr(buf, (islong) ? "S8" : "S9", "XXXXXX", (islong) ? "XX" :
			NULL, NULL);
		sum = putx(&buf[2], (islong) ? 4 : 3);
		i = 4;
		if (islong)
			{
			sum =+ putx(&buf[i], (UCOUNT)(tbias >> 16));
			i =+ 2;
			}
		sum =+ putx(&buf[i], (COUNT)(tbias >> 8));
		i =+ 2;
		sum =+ putx(&buf[i], (COUNT)tbias);
		i =+ 2;
		putx(&buf[i], ~sum);
		i =+ 2;
		buf[i] = '\n';
		++i;
		}
	if (putlin(buf, i) != i)
		error("can't write end record", NULL);
	}

/*	put two hex digits
 */
VOID putx(s, n)
	FAST TEXT *s;
	FAST UTINY n;
	{
	FAST UTINY x;

	x = (n >> 4) & 0xf;
	s[0] = x + ( x < 10 ? '0' : 'A' - 10);
	x = n & 0xf;
	s[1] = x + ( x < 10 ? '0' : 'A' - 10);
	return (n);
	}

/*	get long inproper byte order
 */
LONG xstol(s)
	FAST UTINY *s;
	{
	IMPORT BOOL lsfmt;
	FAST COUNT i;
	FAST LONG val;

	val = 0;
	if (lsfmt)
		for (i = 4; 0 <= --i;)
			val = val << 8 | s[i];
	else
		for (i = 0; i < 4; ++i)
			val = val << 8 | s[i];
	return (val);
	}

/*	set short in proper order
 */
UCOUNT xstos(s)
	FAST UTINY *s;
	{
	IMPORT BOOL lsfmt;
	FAST COUNT val;

	if (lsfmt)
		val = s[0] | s[1] << 8;
	else
		val = s[0] << 8 | s[1];
	return (val);
	}
