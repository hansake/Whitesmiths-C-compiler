/*	CONVERT LOWER CASE LETTER TO UPPER CASE
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
typedef short METACH;

METACH toupper(c)
	METACH c;
	{
	return (islower(c) ? c + 'A' - 'a' : c);
	}
