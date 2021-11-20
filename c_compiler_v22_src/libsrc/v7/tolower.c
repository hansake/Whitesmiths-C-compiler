/*	CONVERT UPPER CASE LETTER TO LOWER CASE
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
typedef short METACH;

METACH tolower(c)
	METACH c;
	{
	return (isupper(c) ? c + 'a' - 'A' : c);
	}
