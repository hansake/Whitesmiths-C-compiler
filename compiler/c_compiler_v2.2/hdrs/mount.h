/*	THE MOUNTED FILESYSTEM HEADER
 * 	copyright (c) 1980 by Whitesmiths, Ltd.
 */

/*	mount table constants
 */
#define MTAB	"/adm/mount"
#define MAXPATH	(NAMSIZE - 1)

/*	an entry in the mounted table
 */
typedef struct {
	UCOUNT m_dev;
	TEXT m_flags;
	TEXT m_path[MAXPATH];
	} MENT;
