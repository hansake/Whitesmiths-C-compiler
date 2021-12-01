#include <wslxa.h>
#include <fcntl.h>

TEXT buf[512];

BOOL main()
	{
	COUNT n;
	IMPORT COUNT _errno;

	for (;;)
		{
		n = read(STDIN, buf, sizeof buf);
		putfmt("n = %i\n", n);
		if (n < 0)
			{
			putfmt("_errno = %i\n", _errno);
			return(1);
			}
		else if (n == 0)
			return (0);
		putfmt("--%b--\n", buf, n - 1);
		}
	return(0);
	}
