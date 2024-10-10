
#ifdef __POWERC
#define _DOS
#endif


/* Common functions that no OS should be without.	*/

/* Common (every OS) Include files.	*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "myfuncs.h"

#ifdef _DOS
/* DOS Include files.	*/
/*
 * #include <process.h> #include <stdlib.h> #include <time.h> #include
 * <errno.h> #include <string.h> #include <sys/types.h>
 *
 * time_t	t_start; time_t	t_finish;
 */
#endif

#ifdef UNIX
/* UNIX Include files.	*/
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#endif

#ifndef NO
#define NO	0
#endif


/* =======================>> str_tolower <<======================= */
char	       *str_toupper(char path[])

{

    int		    index;
    int		    max_index;
    int		    my_char;

    /* Convert any lower-case characters in the specified	*/
    /* path-name to their upper-case equivalents.			*/
    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    if (path[0] == (char)0)
    {
	return (path);	/* Empty path- give it back to caller!	*/
    }

    max_index = strlen(path);
    for (index = 0; index < max_index; index++)
    {
	my_char = (int)path[index];
	if (islower(my_char) != NO)
	{
	    path[index] = (char)(toupper(my_char));
	}
    }

    return (path);	/* Return results to caller.	*/

}

/* =======================>> str_tolower <<======================= */
char	       *str_tolower(char path[])

{

    int		    index;
    int		    max_index;
    int		    my_char;

    /* Convert any upper-case characters in the specified	*/
    /* path-name to their lower-case equivalents.			*/
    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    if (path[0] == (char)0)
    {
	return (path);	/* Empty path- give it back to caller!	*/
    }

    max_index = strlen(path);
    for (index = 0; index < max_index; index++)
    {
	my_char = (int)path[index];
	if (isupper(my_char) != NO)
	{
	    path[index] = (char)(tolower(my_char));
	}
    }

    return (path);	/* Return results to caller.	*/

}

/* =======================>> str_tolower <<======================= */
char	       *dos2unix_path(char path[])

{

    int		    index;
    int		    max_index;

    char	   *root;

    static char	    result[200];

    /* Convert any DOS directory-delimiter characters in	*/
    /* the specified path-name to their UNIX equivalents.	*/
    /* Also, remove any DOS drive-designators.				*/

    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    if (path[0] == (char)0)
    {
	return (path);	/* Empty path- give it back to caller!	*/
    }

    strcpy(result, path);
    root = strrchr(path, 46);	/* Find the final period	*/
    if (root != NULL)
    {
	root++;
	strcpy(result, root);
    }

    max_index = strlen(result);
    for (index = 0; index < max_index; index++)
    {
	if (result[index] == (char)BACKSLASH)
	{
	    result[index] = (char)SLASH;
	}
    }

    return (result);	/* Return results to caller.	*/
}


/* =======================>> str_tolower <<======================= */
char	       *unix2dos_path(char path[])

{

    int		    index;
    int		    max_index;

    /* Convert any UNIX directory-delimiter characters in	*/
    /* the specified path-name to their DOS equivalents.	*/

    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    if (path[0] == (char)0)
    {
	return (path);	/* Empty path- give it back to caller!	*/
    }

    max_index = strlen(path);
    for (index = 0; index < max_index; index++)
    {
	if (path[index] == (char)SLASH)
	{
	    path[index] = (char)BACKSLASH;
	}
    }

    return (path);	/* Return results to caller.	*/
}



/* ======================>> strip_extension <<====================== */
char	       *strip_extension(char path[])

{

    char	   *dot;

    /* Strip the directory components from the specified	*/
    /* path-name, ala the UNIX command of the same name.	*/
    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    if (path[0] == (char)0)
    {
	return (path);	/* Empty path- give it back to caller!	*/
    }

    dot = strrchr(path, 46);	/* Find the final period	*/
    /* in path...				*/
    if (dot != NULL)
    {	/* If one is found, truncate	*/
	*dot = (char)0;	/* path at that point.			*/
    }

    return (path);	/* Return remains to caller.	*/

}


/* =========================>> basename <<========================= */
char *basename(char path[])

{

    int		    index;
    int		    my_char;

    static char	    base[50];

    /* Strip the directory components from the specified	*/
    /* path-name, ala the UNIX command of the same name.	*/
    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    base[0] = (char)0;	/* Clear return-string to start.	*/
    if (path[0] == (char)0)
    {
	return (base);	/* Empty path- give it back to caller!	*/
    }

    index = strlen(path);
    index--;

    do
    {
	my_char = (int)path[index];
	switch (my_char)
	{
	case SLASH:	/* UNIX directory-delimiter.	*/
	case BACKSLASH:	/* DOS directory-delimiter.	*/
	case COLON:	/* DOS drive-designator.		*/
	    index++;
	    strcpy(base, &path[index]);
	    return (base);
	    break;

	default:
	    break;
	}
    } while (index-- > 0);
    /* No delimiters found!  Return whole thing to caller.	*/
    strcpy(base, path);
    return (base);

}





/* =========================>> basename <<========================= */
char *dirname(char path[])

{

    int		    index;
    int		    my_char;

    static char	    base[80];

    /* Strip the directory components from the specified	*/
    /* path-name, ala the UNIX command of the same name.	*/
    if (path == NULL)
    {
	return (NULL);	/* Bogus path- give it back to caller!	*/
    }

    base[0] = (char)0;	/* Clear return-string to start.	*/
    if (path[0] == (char)0)
    {
	return (base);	/* Empty path- give it back to caller!	*/
    }

    index = strlen(path);
    index--;

    do
    {
	my_char = (int)path[index];
	switch (my_char)
	{
	case SLASH:	/* UNIX directory-delimiter.	*/
	case BACKSLASH:	/* DOS directory-delimiter.	*/
	    strncpy(base, path, index);
	    base[index] = (char)0;
	    return (base);
	    break;

	case COLON:	/* DOS drive-designator.		*/
	    index++;
	    strncpy(base, path, index);
	    base[index] = (char)0;
	    return (base);
	    break;

	default:
	    break;
	}
    } while (index-- > 0);
    /* No delimiters found!  Return empty string to caller.	*/
    base[0] = (char)0;	/* Clear return-string to start.	*/
    return (base);

}



/* ===========================>> closefile <<=========================== */
void
closefile(FILE * file)

{

    /* Close the file ONLY if its descriptor is not a NULL	*/
    /* pointer.  In other words, if the file was open.		*/
    if (file != NULL)
    {
	fclose(file);
    }
    /* Then make sure the file-descriptor is	*/
    /* NULL to mark the file as being closed.	*/
    file = NULL;
}


/* =========================>> format_time <<====================== */
char	       *format_time(time_t start, time_t finish)

{

    short	    my_days;
    short	    my_hours;
    short	    my_mins;
    short	    my_secs;
    int		    index;

    time_t	    my_time;

    static char	    time_string[50];

    if ((start < (time_t) 0) || (finish < (time_t) 0) || (finish < start))
    {
	return (NULL);	/* Bogus time(s)- bail!	*/
    }

    my_time = (time_t) finish - start;
    if (my_time == (time_t) 0)
    {
	strcpy(time_string, "< 1(!) Second");
	return (time_string);
    }

    time_string[0] = (char)0;	/* Clear return-string to start.	*/

    /* Days are easy.  Just divide seconds	*/
    /* by number of seconds in a day.		*/
    my_days = (short)(my_time / 86400);

    /* Seconds are easy, too.  Just divide seconds	*/
    /* by number of seconds in a minute.			*/
    my_secs = (short)(my_time % 60);

    /* To calculate hours, use remains of the day	*/
    /* and divide by number of seconds in an hour.	*/
    my_time %= 86400;
    my_hours = (short)(my_time / 3600);

    my_time %= 3600;
    my_mins = (short)(my_time / 60);

    if (my_days > 0)
    {
	if (my_days == 1)
	{
	    strcpy(time_string, "1 Day");
	}
	else
	{
	    sprintf(time_string, "%d Days", my_days);
	}
    }

    index = strlen(time_string);
    if (my_hours > 0)
    {
	if (index > 0)
	{
	    strcat(time_string, ", ");
	    index += 2;
	}
	if (my_hours == 1)
	{
	    strcat(time_string, "1 Hour");
	}
	else
	{
	    sprintf(&time_string[index], "%d Hours", my_hours);
	}
    }

    index = strlen(time_string);
    if (my_mins > 0)
    {
	if (index > 0)
	{
	    strcat(time_string, ", ");
	    index += 2;
	}
	if (my_mins == 1)
	{
	    strcat(time_string, "1 Min");
	}
	else
	{
	    sprintf(&time_string[index], "%d Mins", my_mins);
	}
    }

    index = strlen(time_string);
    if (my_secs > 0)
    {
	if (index > 0)
	{
	    strcat(time_string, ", ");
	    index += 2;
	}
	if (my_secs == 1)
	{
	    strcat(time_string, "1 Sec");
	}
	else
	{
	    sprintf(&time_string[index], "%d Secs", my_secs);
	}
    }

    return (time_string);

}


/* =========================>> basename <<========================= */
void
sort_table(char *table[], int max_count, int offset)

{

    int		    gap;
    int		    i;
    int		    j;
    char	   *temp;

    if (table == NULL)
    {
	return;	/* Bogus table- give it back to caller!	*/
    }

    if (table[0][0] == (char)0)
    {
	return;	/* Empty table- give it back to caller!	*/
    }

    for (gap = (max_count / 2); gap > 0; gap /= 2)
    {
	for (i = gap; i < max_count; i++)
	{
	    for (j = (i - gap); j >= 0; j -= gap)
	    {
		if (strcmp(&table[j][offset], &table[j + gap][offset]) <= 0)
		{
		    break;
		}
		temp = table[j];
		table[j] = table[j + gap];
		table[j + gap] = temp;
	    }
	}
    }

    return;
}
