#include <wslxa.h>
#include <stdio.h>
#include <fcntl.h>

FD fd = {0};
FILE *fp = {0};

BOOL main()
	{

	fd = _open("prt:", O_WRONLY, 0, NULL );
	_write(fd, "hello world: 1\n", 15 );
	fp = fdopen(fd, "w");
	fwrite("hello world: 2\n", 1, 15, fp);
	fputs("hello world: 3\n", fp);
	fprintf(fp, "hello world: 4\n");
	fprintf(fp, "hello world: %i\n", 5);
	putf(fp, "hello world: 6\n");
	putf(fp, "hello world: %i\n", 7);
	fclose(fp);
	}
