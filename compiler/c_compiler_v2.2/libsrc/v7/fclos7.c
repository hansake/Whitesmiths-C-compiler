/*	FCLOSE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID fclos7(fp)
	FIO *fp;
	{
	IMPORT FIO *pstdin, *pstdout, *pstderr;

	fclose(fp);
	if (fp != pstdin && fp != pstdout && fp != pstderr)
		free(fp);
	}
