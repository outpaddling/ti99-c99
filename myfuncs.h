
/*	Common functions that no OS should be without.	*/

		/*	Common (every OS) Include files.	*/

#ifdef _DOS
		/*	DOS Include files.	*/
#endif

#ifdef UNIX
		/*	UNIX Include files.	*/
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

#ifndef YES
	#define YES	TRUE
#endif

#ifndef NO
	#define NO	FALSE
#endif

#ifndef STR_MATCH
	#define STR_MATCH	0
#endif

	/*	Definitions of character ASCII codes.	*/
#ifndef PERIOD
	#define PERIOD		46
#endif

#ifndef SLASH
	#define SLASH		47
#endif

#ifndef COLON
	#define COLON		58
#endif

#ifndef BACKSLASH
	#define BACKSLASH	92
#endif



#ifndef _MYFUNCS
#define _MYFUNCS

#include <time.h>
#include <stdio.h>

char	*str_toupper (char path []);
char	*str_tolower (char path []);
char	*strip_extension (char path []);
char	*basename (char path []);
char	*dirname (char path []);
void	 closefile (FILE *file);
char	*format_time (time_t start, time_t finish);
char	*dos2unix_path (char path []);
char	*unix2dos_path (char path []);
void	 sort_table (char *table [], int max_count, int offset);

#endif	/*	_MYFUNCS	*/

